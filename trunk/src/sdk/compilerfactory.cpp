#include "compilerfactory.h"
#include "manager.h"
#include "messagemanager.h"
#include "configmanager.h"

CompilersArray CompilerFactory::Compilers;

void CompilerFactory::RegisterCompiler(Compiler* compiler)
{
    CompilerFactory::Compilers.Add(compiler);
}

void CompilerFactory::RegisterUserCompilers()
{
	wxString str;
	long cookie;
	
	ConfigManager::Get()->SetPath("/compiler_gcc/compiler_sets");
	bool cont = ConfigManager::Get()->GetFirstGroup(str, cookie);
	while (cont)
	{
		int parent = -1;
		parent = ConfigManager::Get()->Read("/compiler_gcc/compiler_sets/" + str + "/_parent", -1);

        if (CompilerIndexOK(parent - 1))
        {
            CreateCompilerCopy(Compilers[parent - 1]);
        }

		cont = ConfigManager::Get()->GetNextGroup(str, cookie);
	}
	ConfigManager::Get()->SetPath("/");
}

int CompilerFactory::CreateCompilerCopy(Compiler* compiler)
{
    Compiler* newC = compiler->CreateCopy();
    RegisterCompiler(newC);
    newC->LoadSettings("/compiler_gcc/compiler_sets");
    Manager::Get()->GetMessageManager()->DebugLog("Added compiler \"%s\"", newC->GetName().c_str());
    return Compilers.GetCount() - 1; // return the index for the new compiler
}

void CompilerFactory::RemoveCompiler(Compiler* compiler)
{
    int listIdx = compiler->m_ID;
    
    // loop through compilers list and adjust all following compilers m_ID -= 1 and m_ParentID -= 1
    for (unsigned int i = listIdx; i < Compilers.GetCount(); ++i)
    {
        Compiler* tmp = Compilers[i];
        if (tmp->m_ParentID == compiler->m_ID)
        {
            // this compiler has parent the compiler to be deleted
            tmp->m_ParentID = compiler->m_ParentID;
        }
        else if (tmp->m_ParentID >= listIdx)
        {
            tmp->m_ParentID -= 1;
        }
        tmp->m_ID -= 1;
    }
    Compilers.Remove(compiler);
    Manager::Get()->GetMessageManager()->DebugLog("Compiler \"%s\" removed", compiler->GetName().c_str());
    delete compiler;
    
    SaveSettings();
}

void CompilerFactory::UnregisterCompilers()
{
    WX_CLEAR_ARRAY(CompilerFactory::Compilers);
    CompilerFactory::Compilers.Empty();
}

bool CompilerFactory::CompilerIndexOK(int compilerIdx)
{
    return CompilerFactory::Compilers.GetCount() && compilerIdx >= 0 && compilerIdx < (int)CompilerFactory::Compilers.GetCount();
}

void CompilerFactory::SaveSettings()
{
    wxString baseKey = "/compiler_gcc/compiler_sets";
    ConfigManager::Get()->DeleteGroup(baseKey);
    for (unsigned int i = 0; i < Compilers.GetCount(); ++i)
    {
        Compilers[i]->SaveSettings(baseKey);
    }
}

void CompilerFactory::LoadSettings()
{
    wxString baseKey = "/compiler_gcc/compiler_sets";
    for (unsigned int i = 0; i < Compilers.GetCount(); ++i)
    {
        Compilers[i]->LoadSettings(baseKey);
    }
}
