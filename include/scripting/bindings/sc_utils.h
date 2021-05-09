/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef CB_SC_UTILS_H
#define CB_SC_UTILS_H

#include "cbexception.h"

#include <squirrel.h>
#include <string>
#include <sstream>
#include <stdarg.h>
#include <string.h>
#include <memory>
#include <vector>

#ifndef CB_PRECOMP
    #include <wx/string.h>
    #include "prep.h"
#endif // CB_PRECOMP

// FIXME (squirrel) Explain how things below actually work.

namespace ScriptBindings
{

void PrintStack(HSQUIRRELVM vm, const char *title, SQInteger oldTop = -1);
wxString ExtractLastSquirrelError(HSQUIRRELVM vm, bool canBeEmpty);

struct PreserveTop
{
    PreserveTop(HSQUIRRELVM vm) : m_vm(vm), m_top(sq_gettop(vm)) {}
    ~PreserveTop()
    {
        if (m_checkAtDtor)
            check();
        sq_settop(m_vm, m_top);
    }

    void check()
    {
#if defined(cbDEBUG)
        if (sq_gettop(m_vm) != m_top)
            PrintStack(m_vm, "PreserveTop::check failure!", m_top);
#endif
        cbAssert(sq_gettop(m_vm) == m_top);
    }
    void noCheck() { m_checkAtDtor = false; }
private:
    HSQUIRRELVM m_vm;
    SQInteger m_top;
    bool m_checkAtDtor = true;
};

inline bool GetRootTableField(HSQUIRRELVM vm, const SQChar *fieldName)
{
    sq_pushroottable(vm);
    sq_pushstring(vm, fieldName, -1);

    //gets the field 'fieldName' from the root table
    const bool result = SQ_SUCCEEDED(sq_get(vm, -2));
    sq_remove(vm, -2); // remove the root table
    return result;
}

template<typename T>
struct TypeInfo {};

enum class InstanceAllocationMode : uint32_t
{
    InstanceIsInline,
    InstanceIsOwnedPtr,
    InstanceIsNonOwnedPtr
};

constexpr size_t smax(size_t a, size_t b)
{
    return a < b ? b : a;
}

template<typename UserType>
struct UserDataForType
{
    InstanceAllocationMode mode;
    /// Make it possible to store either whole object or a pointer to an object.
    /// Make sure the data is properly aligned.

    union {
        typename std::aligned_storage<
            smax(sizeof(UserType), sizeof(UserType*)),
            smax(alignof(UserType), alignof(UserType*))
        >::type userdata;
        UserType *userptr;
    };
};

template<typename UserType>
SQInteger ReleaseHook(SQUserPointer ptr, cb_unused SQInteger size)
{
    UserDataForType<UserType> &data = *reinterpret_cast<UserDataForType<UserType>*>(ptr);

    switch (data.mode)
    {
    case InstanceAllocationMode::InstanceIsInline:
        {
            UserType *p = reinterpret_cast<UserType*>(&data.userdata);
            p->~UserType();
            break;
        }
    default:
        cbAssert(false);
    }

    return 0;
}

template<typename UserType, InstanceAllocationMode mode>
UserDataForType<UserType>* SetupUserPointer(HSQUIRRELVM vm, SQInteger idx)
{
    SQUserPointer ptr = nullptr;
    if (SQ_FAILED(sq_getinstanceup(vm, idx, &ptr,
                                   SQUserPointer(uint64_t(TypeInfo<UserType>::typetag)))))
    {
        sq_throwerror(vm, _SC("SetupUserPointer: Invalid type tag!"));
        cbAssert(false);
        return nullptr;
    }

    UserDataForType<UserType> *data = reinterpret_cast<UserDataForType<UserType>*>(ptr);
    data->mode = mode;
    // FIXME (squirrel) C++17 feature?!!! Could be replaced with a trait, but should we?
    if constexpr (mode == InstanceAllocationMode::InstanceIsInline)
        sq_setreleasehook(vm, idx, ReleaseHook<UserType>);
    return data;
}

template<typename UserType>
inline bool ExtractUserPointer(UserType *&self, HSQUIRRELVM vm, SQInteger idx, uint32_t tag)
{
    self = nullptr;
    SQUserPointer ptr = nullptr;
    if (SQ_FAILED(sq_getinstanceup(vm, idx, &ptr, SQUserPointer(uint64_t(tag)))))
    {
        //cbAssert(false);
        return false;
    }

    UserDataForType<UserType> &data = *reinterpret_cast<UserDataForType<UserType>*>(ptr);

    switch (data.mode)
    {
    case InstanceAllocationMode::InstanceIsInline:
        self = reinterpret_cast<UserType*>(&data.userdata);
        return self != nullptr;
    case InstanceAllocationMode::InstanceIsOwnedPtr:
    case InstanceAllocationMode::InstanceIsNonOwnedPtr:
        //self = *(UserType**)(ptr + sizeof(InstanceAllocationMode));
        self = data.userptr;
        cbAssert(data.mode!=InstanceAllocationMode::InstanceIsOwnedPtr);
        return self != nullptr;
    }

    return self != nullptr;
}

struct SkipParam {};

struct ExtractParamsBase
{
    ExtractParamsBase(HSQUIRRELVM vm) : m_vm(vm) {}

    template<typename Arg>
    bool ProcessParam(Arg &arg, int stackIndex, const char *funcStr)
    {
        using NoPtrAndCVType = typename std::remove_cv<typename std::remove_pointer<Arg>::type>::type;

        arg = nullptr;
        if (!ExtractUserPointer(arg, m_vm, stackIndex, TypeInfo<NoPtrAndCVType>::typetag))
        {
#if SQUIRREL_VERSION_NUMBER>=300
            scsprintf(m_errorMessage, 500, _SC("Extracting '%s' in '%s' failed for index %d"),
                      typeid(Arg).name(), funcStr, stackIndex);
#else
            scsprintf(m_errorMessage, _SC("Extracting '%s' in '%s' failed for index %d"),
                      typeid(Arg).name(), funcStr, stackIndex);
#endif
            return false;
        }

        return true;
    }

    bool ProcessParam(float &f, int stackIndex, const char *funcStr)
    {
        SQFloat temp;
        if (SQ_FAILED(sq_getfloat(m_vm, stackIndex, &temp)))
        {
#if SQUIRREL_VERSION_NUMBER>=300
            scsprintf(m_errorMessage, 500, _SC("Extract float in '%s' failed for index %d"),
                      funcStr, stackIndex);
#else
            scsprintf(m_errorMessage, _SC("Extract float in '%s' failed for index %d"),
                      funcStr, stackIndex);
#endif
            return false;
        }
        f = temp;
        return true;
    }

    bool ProcessParam(SQInteger &i, int stackIndex, const char *funcStr)
    {
        SQInteger temp;
        if (SQ_FAILED(sq_getinteger(m_vm, stackIndex, &temp)))
        {
#if SQUIRREL_VERSION_NUMBER>=300
            scsprintf(m_errorMessage, 500, _SC("Extract int in '%s' failed for index %d"),
                      funcStr, stackIndex);
#else
            scsprintf(m_errorMessage, _SC("Extract int in '%s' failed for index %d"),
                      funcStr, stackIndex);
#endif
            return false;
        }
        i = temp;
        return true;
    }

    bool ProcessParam(bool &b, int stackIndex, const char *funcStr)
    {
        SQBool temp;
        if (SQ_FAILED(sq_getbool(m_vm, stackIndex, &temp)))
        {
#if SQUIRREL_VERSION_NUMBER>=300
            scsprintf(m_errorMessage, 500, _SC("Extract bool in '%s' failed for index %d"),
                      funcStr, stackIndex);
#else
            scsprintf(m_errorMessage, _SC("Extract bool in '%s' failed for index %d"),
                      funcStr, stackIndex);
#endif
            return false;
        }
        b = temp;
        return true;
    }

    bool ProcessParam(const SQChar *&c, int stackIndex, const char *funcStr)
    {
        const SQChar *temp;
        if (SQ_FAILED(sq_getstring(m_vm, stackIndex, &temp)))
        {
#if SQUIRREL_VERSION_NUMBER>=300
            scsprintf(m_errorMessage, 500, _SC("Extract string in '%s' failed for index %d"),
                      funcStr, stackIndex);
#else
            scsprintf(m_errorMessage, _SC("Extract string in '%s' failed for index %d"),
                      funcStr, stackIndex);
#endif
            return false;
        }

        c = temp;
        return true;
    }

    bool ProcessParam(cb_unused SkipParam &v, cb_unused int stackIndex,
                      cb_unused const char *funcStr)
    {
        // This allows us to skip the param for some reason.
        return true;
    }

    float GetParamFloat(int stackIndex)
    {
        SQFloat value;
        sq_getfloat(m_vm, stackIndex, &value);
        return value;
    }

    SQInteger GetParamInt(int stackIndex)
    {
        SQInteger value;
        sq_getinteger(m_vm, stackIndex, &value);
        return value;
    }

    bool GetParamBool(int stackIndex)
    {
        SQBool value;
        sq_getbool(m_vm, stackIndex, &value);
        return value;
    }

    const SQChar* GetParamString(int stackIndex)
    {
        const SQChar *value;
        sq_getstring(m_vm, stackIndex, &value);
        return value;
    }

    bool CheckNumArguments(int expected, const char *funcStr)
    {
        const int numArgs = sq_gettop(m_vm);
        if (numArgs != expected)
        {
#if SQUIRREL_VERSION_NUMBER>=300
            scsprintf(m_errorMessage, 500,
                      _SC("Wrong number of arguments to '%s' - expected %d given %d (often one argument is an implicit 'this' table)!"),
                      funcStr, expected, numArgs);
#else
            scsprintf(m_errorMessage,
                      _SC("Wrong number of arguments to '%s' - expected %d given %d (often one argument is an implicit 'this' table)!"),
                      funcStr, expected, numArgs);
#endif
            return false;
        }
        return true;
    }

    bool CheckNumArguments(int expectedMin, int expectedMax, const char *funcStr)
    {
        const int numArgs = sq_gettop(m_vm);
        if (numArgs < expectedMin || numArgs > expectedMax)
        {
#if SQUIRREL_VERSION_NUMBER>=300
            scsprintf(m_errorMessage, 500,
                      _SC("Wrong number of arguments to '%s' - expected [%d;%d] given %d (often one argument is an implicit 'this' table)!"),
                      funcStr, expectedMin, expectedMax, numArgs);
#else
            scsprintf(m_errorMessage,
                      _SC("Wrong number of arguments to '%s' - expected [%d;%d] given %d (often one argument is an implicit 'this' table)!"),
                      funcStr, expectedMin, expectedMax, numArgs);
#endif
            return false;
        }
        return true;
    }

    int ErrorMessage()
    {
        return sq_throwerror(m_vm, m_errorMessage);
    }

    HSQUIRRELVM GetVM() { return m_vm; }

protected:
    HSQUIRRELVM m_vm;
    SQChar m_errorMessage[500];
};

template<typename Arg0>
struct ExtractParams1 : ExtractParamsBase
{
    ExtractParams1(HSQUIRRELVM vm) : ExtractParamsBase(vm) {}

    bool Process(const char *funcStr)
    {
        if (!CheckNumArguments(1, funcStr))
            return false;
        if (!ProcessParam(p0, 1, funcStr))
            return false;
        return true;
    }

    Arg0 p0;
};

template<typename Arg0, typename Arg1>
struct ExtractParams2 : ExtractParamsBase
{
    ExtractParams2(HSQUIRRELVM vm) : ExtractParamsBase(vm) {}

    bool Process(const char *funcStr)
    {
        if (!CheckNumArguments(2, funcStr))
            return false;
        if (!ProcessParam(p0, 1, funcStr))
            return false;
        if (!ProcessParam(p1, 2, funcStr))
            return false;
        return true;
    }

    Arg0 p0;
    Arg1 p1;
};

template<typename Arg0, typename Arg1, typename Arg2>
struct ExtractParams3 : ExtractParamsBase
{
    ExtractParams3(HSQUIRRELVM vm) : ExtractParamsBase(vm) {}

    bool Process(const char *funcStr)
    {
        if (!CheckNumArguments(3, funcStr))
            return false;
        if (!ProcessParam(p0, 1, funcStr))
            return false;
        if (!ProcessParam(p1, 2, funcStr))
            return false;
        if (!ProcessParam(p2, 3, funcStr))
            return false;
        return true;
    }

    Arg0 p0;
    Arg1 p1;
    Arg2 p2;
};

template<typename Arg0, typename Arg1, typename Arg2, typename Arg3>
struct ExtractParams4 : ExtractParamsBase
{
    ExtractParams4(HSQUIRRELVM vm) : ExtractParamsBase(vm) {}

    bool Process(const char *funcStr)
    {
        if (!CheckNumArguments(4, funcStr))
            return false;
        if (!ProcessParam(p0, 1, funcStr))
            return false;
        if (!ProcessParam(p1, 2, funcStr))
            return false;
        if (!ProcessParam(p2, 3, funcStr))
            return false;
        if (!ProcessParam(p3, 4, funcStr))
            return false;
        return true;
    }

    Arg0 p0;
    Arg1 p1;
    Arg2 p2;
    Arg3 p3;
};

template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
struct ExtractParams5 : ExtractParamsBase
{
    ExtractParams5(HSQUIRRELVM vm) : ExtractParamsBase(vm) {}

    bool Process(const char *funcStr)
    {
        if (!CheckNumArguments(5, funcStr))
            return false;
        if (!ProcessParam(p0, 1, funcStr))
            return false;
        if (!ProcessParam(p1, 2, funcStr))
            return false;
        if (!ProcessParam(p2, 3, funcStr))
            return false;
        if (!ProcessParam(p3, 4, funcStr))
            return false;
        if (!ProcessParam(p4, 5, funcStr))
            return false;
        return true;
    }

    Arg0 p0;
    Arg1 p1;
    Arg2 p2;
    Arg3 p3;
    Arg4 p4;
};

template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
struct ExtractParams6 : ExtractParamsBase
{
    ExtractParams6(HSQUIRRELVM vm) : ExtractParamsBase(vm) {}

    bool Process(const char *funcStr)
    {
        if (!CheckNumArguments(6, funcStr))
            return false;
        if (!ProcessParam(p0, 1, funcStr))
            return false;
        if (!ProcessParam(p1, 2, funcStr))
            return false;
        if (!ProcessParam(p2, 3, funcStr))
            return false;
        if (!ProcessParam(p3, 4, funcStr))
            return false;
        if (!ProcessParam(p4, 5, funcStr))
            return false;
        if (!ProcessParam(p5, 6, funcStr))
            return false;
        return true;
    }

    Arg0 p0;
    Arg1 p1;
    Arg2 p2;
    Arg3 p3;
    Arg4 p4;
    Arg4 p5;
};

template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5,
         typename Arg6>
struct ExtractParams7 : ExtractParamsBase
{
    ExtractParams7(HSQUIRRELVM vm) : ExtractParamsBase(vm) {}

    bool Process(const char *funcStr)
    {
        if (!CheckNumArguments(7, funcStr))
            return false;
        if (!ProcessParam(p0, 1, funcStr))
            return false;
        if (!ProcessParam(p1, 2, funcStr))
            return false;
        if (!ProcessParam(p2, 3, funcStr))
            return false;
        if (!ProcessParam(p3, 4, funcStr))
            return false;
        if (!ProcessParam(p4, 5, funcStr))
            return false;
        if (!ProcessParam(p5, 6, funcStr))
            return false;
        if (!ProcessParam(p6, 7, funcStr))
            return false;
        return true;
    }

    Arg0 p0;
    Arg1 p1;
    Arg2 p2;
    Arg3 p3;
    Arg4 p4;
    Arg5 p5;
    Arg6 p6;
};

template<typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5,
         typename Arg6, typename Arg7>
struct ExtractParams8 : ExtractParamsBase
{
    ExtractParams8(HSQUIRRELVM vm) : ExtractParamsBase(vm) {}

    bool Process(const char *funcStr)
    {
        if (!CheckNumArguments(8, funcStr))
            return false;
        if (!ProcessParam(p0, 1, funcStr))
            return false;
        if (!ProcessParam(p1, 2, funcStr))
            return false;
        if (!ProcessParam(p2, 3, funcStr))
            return false;
        if (!ProcessParam(p3, 4, funcStr))
            return false;
        if (!ProcessParam(p4, 5, funcStr))
            return false;
        if (!ProcessParam(p5, 6, funcStr))
            return false;
        if (!ProcessParam(p6, 7, funcStr))
            return false;
        if (!ProcessParam(p7, 8, funcStr))
            return false;
        return true;
    }

    Arg0 p0;
    Arg1 p1;
    Arg2 p2;
    Arg3 p3;
    Arg4 p4;
    Arg5 p5;
    Arg6 p6;
    Arg7 p7;
};

template<typename UserType>
inline SQInteger CreateClassDecl(HSQUIRRELVM v, const SQChar *className,
                                 const SQChar *baseClassName = nullptr)
{
    const SQInteger tableStackIdx = sq_gettop(v);
    cbAssert(scstrcmp(className, TypeInfo<UserType>::className)==0);
    sq_pushstring(v, className, -1);
    if (baseClassName)
    {
        using BaseClass = typename TypeInfo<UserType>::baseClass;
        cbAssert(std::is_void<BaseClass>::value == false);
        if constexpr (std::is_void<BaseClass>::value == false)
            cbAssert(scstrcmp(baseClassName, TypeInfo<BaseClass>::className)==0);

        sq_pushstring(v, baseClassName, -1);
        if (SQ_FAILED(sq_get(v, -3)))
        {
            cbAssert(false);
            return -1;
        }
    }
    else
    {
        cbAssert(std::is_void<typename TypeInfo<UserType>::baseClass>::value == true);
    }

    sq_newclass(v, ((baseClassName != nullptr) ? SQTrue : SQFalse));
    sq_settypetag(v, -1, SQUserPointer(uint64_t(TypeInfo<UserType>::typetag)));
    // Add some memory to the class. We over-allocate in cases where we store only a pointer.
    sq_setclassudsize(v, -1, sizeof(UserDataForType<UserType>));

    return tableStackIdx;
}

/// Creates a squirrel UserType instance and pushes it on the stack.
/// The C++ object/memory is not initialized. The caller must do it before proceeding.
template<typename UserType>
inline UserDataForType<UserType>* CreateInlineInstance(HSQUIRRELVM v)
{
    if (!GetRootTableField(v, TypeInfo<UserType>::className))
    {
        cbAssert(false);
        sq_throwerror(v, _SC("CreateInlineInstance: Getting class name failed!"));
        return nullptr;
    }
    sq_createinstance(v, -1);
    sq_remove(v, -2); // remove class object

    return SetupUserPointer<UserType, InstanceAllocationMode::InstanceIsInline>(v, -1);
}

/// Helper used when you need to copy construct new instance and then return it to Squirrel.
template<typename UserType>
int ConstructAndReturnInstance(HSQUIRRELVM v, const UserType &value)
{
    UserDataForType<UserType> *data = CreateInlineInstance<UserType>(v);
    if (data == nullptr)
        return -1; // An error should have been logged already.
    new (&data->userdata) UserType(value);
    return 1;
}

/// Create an instance of a given type which references some native memory.
/// This creates a unique squirrel object every time the method is called.
/// The C++ memory is the same for all object and it allows modifications.
template<typename UserType>
inline UserDataForType<UserType>* CreateNonOwnedPtrInstance(HSQUIRRELVM v, UserType *value)
{
    if (value == nullptr)
    {
        sq_throwerror(v, _SC("CreateNonOwnedPtrInstance given a null pointer. This is invalid!"));
        return nullptr;
    }

    if (!GetRootTableField(v, TypeInfo<UserType>::className))
    {
        cbAssert(false);
        sq_throwerror(v, _SC("CreateNonOwnedPtrInstance: Getting class name failed!"));
        return nullptr;
    }
    sq_createinstance(v, -1);
    sq_remove(v, -2); // remove class object

    UserDataForType<UserType> *data;
    data = SetupUserPointer<UserType, InstanceAllocationMode::InstanceIsNonOwnedPtr>(v, -1);
    if (data)
        data->userptr = value;
    return data;
}

/// Helper used when you need to create and return a Squirrel instance pointing to some native
/// object. The object must not be nullptr. If it is a Squirrel error is thrown.
template<typename UserType>
int ConstructAndReturnNonOwnedPtr(HSQUIRRELVM v, UserType *value)
{
    if (value == nullptr)
        return sq_throwerror(v, _SC("Returning an instance to a null pointer. This is invalid!"));
    UserDataForType<UserType> *data = CreateNonOwnedPtrInstance<UserType>(v, value);
    if (data == nullptr)
        return -1; // An error should have been logged already.
    return 1;
}

/// Helper used when you need to create and return a Squirrel instance pointing to some native
/// object. This version must be used when the pointer can be null. In this case "null" is passed to
/// Squirrel instead of an instance storing nullptr.
template<typename UserType>
int ConstructAndReturnNonOwnedPtrOrNull(HSQUIRRELVM v, UserType *value)
{
    if (value == nullptr)
    {
        sq_pushnull(v);
        return 1;
    }
    UserDataForType<UserType> *data = CreateNonOwnedPtrInstance<UserType>(v, value);
    if (data == nullptr)
        return -1; // An error should have been logged already.
    return 1;
}

/// Get a reference to an object in the middle of the stack and copy it at the top.
/// Hopefully this handles internal references correctly.
inline void DuplicateInstance(HSQUIRRELVM v, SQInteger idx)
{
    HSQOBJECT obj;
    sq_resetobject(&obj);
    sq_getstackobj(v, idx, &obj);
    // Skipping addref and release. Seems to work without them. I hope it is reliable.
    sq_pushobject(v, obj);
}

/// This expects the class to be on the stack.
inline void BindMethod(HSQUIRRELVM v, const SQChar *name, SQFUNCTION func, const SQChar *humanName)
{
    sq_pushstring(v, name, -1);
    sq_newclosure(v, func, 0);
    if (humanName)
        sq_setnativeclosurename(v, -1, humanName);
    sq_newslot(v, -3, SQFalse);
}

/// This expects the class to be on the stack.
inline void BindStaticMethod(HSQUIRRELVM v, const SQChar *name, SQFUNCTION func, const SQChar *humanName)
{
    sq_pushstring(v, name, -1);
    sq_newclosure(v, func, 0);
    if (humanName)
        sq_setnativeclosurename(v, -1, humanName);
    sq_newslot(v, -3, SQTrue);
}

template<typename UserType>
SQInteger Generic_DefaultCtor(HSQUIRRELVM v)
{
    const int numArgs = sq_gettop(v);
    if (numArgs != 1)
        return sq_throwerror(v, _SC("Generic_DefaultCtor: Incorrect number of arguments!"));

    UserDataForType<UserType> *data;
    data = SetupUserPointer<UserType, InstanceAllocationMode::InstanceIsInline>(v, 1);
    if (!data)
        return -1; // SetupUserPointer should have called sq_throwerror!
    new (&(data->userdata)) UserType();

    return 0;
}

template<typename UserType>
SQInteger Generic_InstanceCmp(HSQUIRRELVM v)
{
    ExtractParams2<UserType*, SkipParam> extractor(v);
    if (!extractor.Process("Generic_InstanceCmp"))
        return extractor.ErrorMessage();

    UserType *b;
    if (!ExtractUserPointer(b, v, 2, TypeInfo<UserType>::typetag))
    {
        // Incorrect type, just return that instances doesn't match.
        sq_pushinteger(v, 1);
        return 1;
    }

    sq_pushinteger(v, (extractor.p0 == b ? 0 : 1));
    return 1;
}

template<typename UserType>
SQInteger Generic_InstanceClone(HSQUIRRELVM v)
{
    // new object, old object
    ExtractParams2<SkipParam, const UserType*> extractor(v);
    if (!extractor.Process("Generic_InstanceClone"))
        return extractor.ErrorMessage();

    UserDataForType<UserType> *data;
    data = SetupUserPointer<UserType, InstanceAllocationMode::InstanceIsInline>(v, 1);
    if (!data)
        return -1; // SetupUserPointer should have called sq_throwerror!
    new (&(data->userdata)) UserType(*extractor.p1);
    return 0;
}

template<typename UserType>
void BindEmptyCtor(HSQUIRRELVM v)
{
    BindMethod(v, _SC("constructor"), Generic_DefaultCtor<UserType>, nullptr);
}

template<typename UserType>
void BindDefaultInstanceCmp(HSQUIRRELVM v)
{
    BindMethod(v, _SC("_cmp"), Generic_InstanceCmp<UserType>, nullptr);
}

template<typename UserType>
void BindDefaultClone(HSQUIRRELVM v)
{
    BindMethod(v, _SC("_cloned"), Generic_InstanceClone<UserType>, nullptr);
}

template<typename ReturnType, typename ClassType, ReturnType (ClassType::*func)() const>
SQInteger NoParamGetterInt(HSQUIRRELVM v)
{
    ExtractParams1<const ClassType*> extractor(v);
    if (!extractor.Process("NoParamGetter"))
        return extractor.ErrorMessage();
    sq_pushinteger(v, (extractor.p0->*func)());
    return 1;
}

inline SQInteger ThrowIndexNotFound(HSQUIRRELVM v)
{
#if SQUIRREL_VERSION_NUMBER >= 300
    sq_pushnull(v);
    sq_throwobject(v); // no throw object in 2.x, but it seems we don't need it!?
#endif
    return -1;
}

enum class MatchResult
{
    Found,
    Error,
    NotFound
};

template<typename ClassType>
struct MemberBase
{
    MemberBase(const SQChar *name) : name(name)
    {}
    virtual ~MemberBase() {}

    MatchResult MatchAndGet(HSQUIRRELVM v, const ClassType *instance, const SQChar *nameToMatch)
    {
        if (scstrcmp(name, nameToMatch) == 0)
        {
            if (DoPush(v, instance))
                return MatchResult::Found;
            else
                return MatchResult::Error;
        }
        else
            return MatchResult::NotFound;
    }
    MatchResult MatchAndSet(HSQUIRRELVM v, ClassType *instance, const SQChar *nameToMatch,
                            SQInteger valueIndex)
    {
        if (scstrcmp(name, nameToMatch) == 0)
        {
            if (DoSet(v, instance, valueIndex))
                return MatchResult::Found;
            else
                return MatchResult::Error;
        }
        else
            return MatchResult::NotFound;
    }

    virtual bool DoPush(HSQUIRRELVM v, const ClassType *instance) = 0;
    virtual bool DoSet(HSQUIRRELVM v, ClassType *instance, SQInteger valueIndex) = 0;

protected:
    const SQChar *name;
};

template<typename ClassType>
struct MemberBool : MemberBase<ClassType>
{
    using MemberType = bool ClassType::*;

    MemberBool(const SQChar *name, MemberType memberPtr) :
        MemberBase<ClassType>(name),
        memberPtr(memberPtr)
    {
    }

    bool DoPush(HSQUIRRELVM v, const ClassType *instance) override
    {
        sq_pushbool(v, instance->*memberPtr);
        return true;
    }
    bool DoSet(HSQUIRRELVM v, ClassType *instance, SQInteger valueIndex) override
    {
        SQBool value;
        sq_getbool(v, valueIndex, &value);
        instance->*memberPtr = value;
        return true;
    }

private:
    MemberType memberPtr;
};

// FIXME (squirrel): Add more serious range checking
template<typename ClassType>
struct MemberInt : MemberBase<ClassType>
{
    using MemberType = int ClassType::*;

    MemberInt(const SQChar *name, MemberType memberPtr) :
        MemberBase<ClassType>(name),
        memberPtr(memberPtr)
    {
    }

    bool DoPush(HSQUIRRELVM v, const ClassType *instance) override
    {
        sq_pushinteger(v, instance->*memberPtr);
        return true;
    }
    bool DoSet(HSQUIRRELVM v, ClassType *instance, SQInteger valueIndex) override
    {
        SQInteger value;
        sq_getinteger(v, valueIndex, &value);
        instance->*memberPtr = value;
        return true;
    }

private:
    MemberType memberPtr;
};

// FIXME (squirrel): Add more serious range checking
template<typename ClassType, typename UIntType>
struct MemberUInt : MemberBase<ClassType>
{
    using MemberType = UIntType ClassType::*;

    MemberUInt(const SQChar *name, MemberType memberPtr) :
        MemberBase<ClassType>(name),
        memberPtr(memberPtr)
    {
    }

    bool DoPush(HSQUIRRELVM v, const ClassType *instance) override
    {
        sq_pushinteger(v, instance->*memberPtr);
        return true;
    }
    bool DoSet(HSQUIRRELVM v, ClassType *instance, SQInteger valueIndex) override
    {
        SQInteger value;
        sq_getinteger(v, valueIndex, &value);
        if (value < 0)
        {
            sq_throwerror(v, _SC("Cannot set unsigned member to negative value!"));
            return false;
        }
        instance->*memberPtr = value;
        return true;
    }

private:
    MemberType memberPtr;
};

template<typename ClassType>
struct MemberFloat : MemberBase<ClassType>
{
    using MemberType = float ClassType::*;

    MemberFloat(const SQChar *name, MemberType memberPtr) :
        MemberBase<ClassType>(name),
        memberPtr(memberPtr)
    {
    }

    bool DoPush(HSQUIRRELVM v, const ClassType *instance) override
    {
        sq_pushfloat(v, instance->*memberPtr);
        return true;
    }
    bool DoSet(HSQUIRRELVM v, ClassType *instance, SQInteger valueIndex) override
    {
        SQFloat value;
        sq_getfloat(v, valueIndex, &value);
        instance->*memberPtr = value;
        return true;
    }
private:
    MemberType memberPtr;
};

template<typename ClassType, typename RefType>
struct MemberRef : MemberBase<ClassType>
{
    using MemberType = RefType ClassType::*;

    MemberRef(const SQChar *name, MemberType memberPtr) :
        MemberBase<ClassType>(name),
        memberPtr(memberPtr)
    {
    }

    bool DoPush(HSQUIRRELVM v, const ClassType *instance) override
    {
        RefType *refPtr = &(const_cast<ClassType*>(instance)->*memberPtr);
        if (CreateNonOwnedPtrInstance<RefType>(v, refPtr) == nullptr)
            return false;
        return true;
    }
    bool DoSet(HSQUIRRELVM v, ClassType *instance, SQInteger valueIndex) override
    {
        RefType *ref;
        ExtractParamsBase extractor(v);
        if (!extractor.ProcessParam(ref, valueIndex, "Cannot parse RefType"))
        {
            extractor.ErrorMessage();
            return false;
        }

        instance->*memberPtr = *ref;
        return true;
    }
private:
    MemberType memberPtr;
};

template<typename ClassType>
using MembersType=std::vector<std::unique_ptr<MemberBase<ClassType>>>;

template<typename ClassType>
struct FindMembers
{
    static MembersType<ClassType> members;
};

template<typename ClassType>
void addMemberBool(MembersType<ClassType> &members, const SQChar *name,
                   bool ClassType::*memberPtr)
{
    members.emplace_back(new MemberBool<ClassType>(name, memberPtr));
}

template<typename ClassType>
void addMemberInt(MembersType<ClassType> &members, const SQChar *name,
                  int ClassType::*memberPtr)
{
    members.emplace_back(new MemberInt<ClassType>(name, memberPtr));
}
template<typename ClassType, typename UIntType>
void addMemberUInt(MembersType<ClassType> &members, const SQChar *name,
                   UIntType ClassType::*memberPtr)
{
    members.emplace_back(new MemberUInt<ClassType, UIntType>(name, memberPtr));
}

template<typename ClassType>
void addMemberFloat(MembersType<ClassType> &members, const SQChar *name,
                    float ClassType::* memberPtr)
{
    members.emplace_back(new MemberFloat<ClassType>(name, memberPtr));
}

template<typename ClassType, typename RefType>
void addMemberRef(MembersType<ClassType> &members, const SQChar *name,
                  RefType ClassType::* memberPtr)
{
    members.emplace_back(new MemberRef<ClassType, RefType>(name, memberPtr));
}

template<typename ClassType>
SQInteger GenericMember_get(HSQUIRRELVM v)
{
    ExtractParams2<const ClassType*, const SQChar*> extractor(v);
    if (!extractor.Process("GenericMember_get"))
        return extractor.ErrorMessage();

    for (std::unique_ptr<MemberBase<ClassType>> &member : FindMembers<ClassType>::members)
    {
        switch (member->MatchAndGet(v, extractor.p0, extractor.p1))
        {
            case MatchResult::Found:
                return 1;
            case MatchResult::Error:
                return -1;
            case MatchResult::NotFound:
                break;
        }
    }
    return GenericMember_get<typename TypeInfo<ClassType>::baseClass>(v);
}

template<typename ClassType>
SQInteger GenericMember_set(HSQUIRRELVM v)
{
    ExtractParams3<ClassType*, const SQChar*, SkipParam> extractor(v);
    if (!extractor.Process("GenericMember_set"))
        return extractor.ErrorMessage();

    for (std::unique_ptr<MemberBase<ClassType>> &member : FindMembers<ClassType>::members)
    {
        switch (member->MatchAndSet(v, extractor.p0, extractor.p1, 3))
        {
            case MatchResult::Found:
                return 0;
            case MatchResult::Error:
                return -1;
            case MatchResult::NotFound:
                break;
        }
    }

    return GenericMember_set<typename TypeInfo<ClassType>::baseClass>(v);
}

template<>
inline SQInteger GenericMember_get<void>(HSQUIRRELVM v)
{
    return ThrowIndexNotFound(v);
}

template<>
inline SQInteger GenericMember_set<void>(HSQUIRRELVM v)
{
    return ThrowIndexNotFound(v);
}

/// Helper which sets the functions for getting/setting members of a class.
/// @return The members container which should be populated by the caller.
/// @see addMemberInt, addMemberFloat, addMemberRef
template<typename UserType>
MembersType<UserType>& BindMembers(HSQUIRRELVM v)
{
    SQChar nameBuf[100];
    scsprintf(nameBuf, cbCountOf(nameBuf), _SC("%s::_get"), TypeInfo<UserType>::className);
    BindMethod(v, _SC("_get"), GenericMember_get<UserType>, nameBuf);
    scsprintf(nameBuf, cbCountOf(nameBuf), _SC("%s::_set"), TypeInfo<UserType>::className);
    BindMethod(v, _SC("_set"), GenericMember_set<UserType>, nameBuf);

    return FindMembers<UserType>::members;
}

struct ObjectHandle
{
    ObjectHandle() : m_vm(nullptr)
    {
        sq_resetobject(&m_object);
    }
    ObjectHandle(HSQUIRRELVM vm, SQInteger stackIdx) : m_vm(vm)
    {
        cbAssert(m_vm);
        sq_getstackobj(m_vm, stackIdx, &m_object);
        sq_addref(m_vm, &m_object);
    }
    ObjectHandle(const ObjectHandle &obj) : m_vm(obj.m_vm), m_object(obj.m_object)
    {
        if (m_vm)
            sq_addref(m_vm, &m_object);
    }

    ~ObjectHandle()
    {
        if (m_vm)
            sq_release(m_vm, &m_object);
    }

    ObjectHandle& operator=(const ObjectHandle &obj)
    {
        cbAssert(this != &obj);
        HSQOBJECT temp = obj.m_object; // We need this because obj is const.
        if (obj.m_vm)
            sq_addref(obj.m_vm, &temp);

        if (m_vm)
            sq_release(m_vm, &m_object);
        m_vm = obj.m_vm;
        m_object = temp;

        return *this;
    }

    const HSQOBJECT& Get() const { return m_object; }

    void Push()
    {
        cbAssert(m_vm);
        sq_pushobject(m_vm, m_object);
    }

    HSQUIRRELVM GetVM() { return m_vm; }

private:
    HSQUIRRELVM m_vm;
    HSQOBJECT m_object;
};

/// Simple wrapper for calling Squirrel functions.
/// It supports passing parameter and extracting results (both basic and instance types are
/// supported, if something is missing probably it will be easy to add it).
/// It supports calling global functions and methods.
///
/// There are two main modes of operation and one if you really want to have full control:
/// 1. Checking if the function exists and then call it one or more times. This is achieved by first
///    calling SetupFunc and then calling one of the CallXXX function which doesn't have a function
///    name parameter.
/// 2. Calling functions by name. Call one of the CallByNameXXX functions. It is better to use these
///    if you're going to call this function once.
/// 3. Use the SetupFunc, Push, CallRaw, ExtractResult. This should be used only if any of the other
///    methods is not suitable.
struct Caller
{
    /// Use this if you want to call a global function and pass the root table as the environment
    /// table parameter.
    Caller(HSQUIRRELVM vm) : m_vm(vm), m_closureStackIdx(-1)
    {
        sq_pushroottable(m_vm);
        sq_resetobject(&m_object);
        sq_getstackobj(m_vm, -1, &m_object);
        sq_addref(m_vm, &m_object);

        sq_poptop(m_vm); // pop root table
    }

    /// Use this if you want to call a method on some object.
    Caller(HSQUIRRELVM vm, const HSQOBJECT &object) : m_vm(vm), m_closureStackIdx(-1)
    {
        m_object = object;

        sq_addref(m_vm, &m_object);
    }

    Caller(Caller&) = delete;
    Caller& operator=(Caller&) = delete;

    ~Caller()
    {
        Finish();

        sq_release(m_vm, &m_object);
    }

    /// Call this if you want to reuse the caller object to call another function.
    void Finish()
    {
        // Remove the closure from the stack.
        if (m_closureStackIdx != -1)
        {
            sq_pop(m_vm, sq_gettop(m_vm) - m_closureStackIdx + 1);
            m_closureStackIdx = -1;
        }
    }

    /// Prepare the function for calling.
    /// @return true if successful and false if there is a problem (function not found, object not a
    ///   closure, etc).
    bool SetupFunc(const SQChar *funcName) {
        cbAssert(m_closureStackIdx == -1);
        sq_pushobject(m_vm, m_object);
        sq_pushstring(m_vm, funcName, -1);

        // Gets the field 'funcName' from the object
        if (SQ_FAILED(sq_get(m_vm, -2)))
        {
            sq_poptop(m_vm); // remove the object
            return false;
        }

        sq_remove(m_vm, -2); // remove the object
        if (sq_gettype(m_vm, -1) != OT_CLOSURE)
        {
            sq_poptop(m_vm); // remove the value
            return false;
        }
        m_closureStackIdx = sq_gettop(m_vm);
        return true;
    }

    template<typename Arg>
    bool Push(const Arg *arg)
    {
        if (arg == nullptr)
        {
            sq_pushnull(m_vm);
            return true;
        }
        else
            return CreateNonOwnedPtrInstance(m_vm, const_cast<Arg*>(arg)) != nullptr;
    }

    bool Push(SQInteger arg)
    {
        sq_pushinteger(m_vm, arg);
        return true;
    }

    bool CallRaw(bool hasReturn)
    {
        if (m_closureStackIdx == -1)
        {
            cbAssert(false);
            return false;
        }
        sq_reseterror(m_vm);
        if (SQ_FAILED(sq_call(m_vm, sq_gettop(m_vm) - m_closureStackIdx, hasReturn, SQTrue)))
        {
            sq_poptop(m_vm);
            return false;
        }
        return true;
    }

    template<typename Return>
    bool CallByNameAndReturn0(const SQChar *functionName, Return &returnValue)
    {
        if (!SetupFunc(functionName))
            return false;
        return CallAndReturn0(returnValue);
    }

    template<typename Return, typename Arg0>
    bool CallByNameAndReturn1(const SQChar *functionName, Return &returnValue, Arg0 arg0)
    {
        if (!SetupFunc(functionName))
            return false;
        return CallAndReturn1(returnValue, arg0);
    }

    template<typename Return>
    bool CallAndReturn0(Return &returnValue)
    {
        if (!SetupCall())
            return false;

        if (!CallRaw(true))
            return false;
        return ExtractResult(returnValue);
    }

    template<typename Return, typename Arg0>
    bool CallAndReturn1(Return &returnValue, Arg0 arg0)
    {
        if (!SetupCall())
            return false;

        if (!Push(arg0))
            return false;

        if (!CallRaw(true))
            return false;
        return ExtractResult(returnValue);
    }

    template<typename Return, typename Arg0, typename Arg1>
    bool CallByNameAndReturn2(const SQChar *functionName, Return &returnValue, Arg0 arg0, Arg1 arg1)
    {
        if (!SetupFunc(functionName))
            return false;
        if (!SetupCall())
            return false;

        if (!Push(arg0))
            return false;
        if (!Push(arg1))
            return false;

        if (!CallRaw(true))
            return false;
        return ExtractResult(returnValue);
    }

    template<typename Return, typename Arg0, typename Arg1, typename Arg2>
    bool CallByNameAndReturn3(const SQChar *functionName, Return &returnValue, Arg0 arg0, Arg1 arg1,
                              Arg2 arg2)
    {
        if (!SetupFunc(functionName))
            return false;
        if (!SetupCall())
            return false;

        if (!Push(arg0))
            return false;
        if (!Push(arg1))
            return false;
        if (!Push(arg2))
            return false;

        if (!CallRaw(true))
            return false;
        return ExtractResult(returnValue);
    }

    bool CallByName0(const SQChar *functionName)
    {
        if (!SetupFunc(functionName))
            return false;
        if (!SetupCall())
            return false;

        return CallRaw(false);
    }

    bool Call0()
    {
        if (!SetupCall())
            return false;
        return CallRaw(false);
    }

    template<typename Arg0>
    bool CallByName1(const SQChar *functionName, Arg0 arg0)
    {
        if (!SetupFunc(functionName))
            return false;
        if (!SetupCall())
            return false;

        if (!Push(arg0))
            return false;

        return CallRaw(false);
    }

    template<typename Arg0>
    bool Call1(Arg0 arg0)
    {
        if (!SetupCall())
            return false;
        if (!Push(arg0))
            return false;
        return CallRaw(false);
    }

    template<typename Arg0, typename Arg1>
    bool CallByName2(const SQChar *functionName, Arg0 arg0, Arg1 arg1)
    {
        if (!SetupFunc(functionName))
            return false;
        if (!SetupCall())
            return false;

        if (!Push(arg0))
            return false;
        if (!Push(arg1))
            return false;

        return CallRaw(false);
    }

    template<typename Arg0, typename Arg1, typename Arg2>
    bool CallByName3(const SQChar *functionName, Arg0 arg0, Arg1 arg1, Arg2 arg2)
    {
        if (!SetupFunc(functionName))
            return false;
        if (!SetupCall())
            return false;

        if (!Push(arg0))
            return false;
        if (!Push(arg1))
            return false;
        if (!Push(arg2))
            return false;

        return CallRaw(false);
    }

    bool ExtractResult(SQInteger &result)
    {
        if (SQ_FAILED(sq_getinteger(m_vm, -1, &result)))
            return false;
        sq_poptop(m_vm);
        return true;
    }

    bool ExtractResult(bool &result)
    {
        SQBool value;
        if (SQ_FAILED(sq_getbool(m_vm, -1, &value)))
            return false;
        result = value;
        sq_poptop(m_vm);
        return true;
    }

    template<typename Arg>
    bool ExtractResult(Arg *&arg)
    {
        return ExtractUserPointer(arg, m_vm, -1, TypeInfo<typename std::remove_cv<Arg>::type>::typetag);
    }

    /// Call this if you want to be able to call the function again without the need of a SetupFunc
    /// call. It is an error if you call this after a function call which has no return value.
    void PopResult()
    {
        cbAssert(sq_gettop(m_vm) > m_closureStackIdx);
        sq_pop(m_vm, sq_gettop(m_vm) - m_closureStackIdx);
    }
private:
    bool SetupCall()
    {
        if (m_closureStackIdx != sq_gettop(m_vm))
        {
            cbAssert(false);
            return false;
        }
        sq_pushobject(m_vm, m_object);
        return true;
    }
private:
    HSQUIRRELVM m_vm;
    SQInteger m_closureStackIdx;
    HSQOBJECT m_object;
};

} // namespace ScriptBindings

#endif // CB_SC_UTILS_H
