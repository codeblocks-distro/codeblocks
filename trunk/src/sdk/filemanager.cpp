#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "filemanager.h"
    #include "safedelete.h"
    #include "cbeditor.h"
    #include "editormanager.h"
#endif

#include <wx/url.h>

LoaderBase::~LoaderBase()
{
    delete[] data;
};

bool LoaderBase::Sync()
{
    WaitReady();
    return data;
};

char* LoaderBase::GetData()
{
    WaitReady();
    return data;
};

size_t LoaderBase::GetLength()
{
    WaitReady();
    return len;
};



void FileLoader::operator()()
{
    if(!wxFile::Access(fileName, wxFile::read))
        {
        Ready();
        return;
        }

    wxFile file(fileName);
    len = file.Length();

    data = new char[len+1];
    data[len] = '\0';

    if(file.Read(data, len) == wxInvalidOffset)
    {
        delete[] data;
        data = 0;
        len = 0;
    }
    Ready();
}

void URLLoader::operator()()
{
    wxURL url(fileName);
    url.SetProxy(ConfigManager::GetProxy());

    if (url.GetError() != wxURL_NOERR)
        {
        Ready();
        return;
        }

    std::auto_ptr<wxInputStream> stream(url.GetInputStream());

    if (stream.get() == 0 || stream->IsOk() == false)
        {
        Ready();
        return;
        }

    char tmp[8192];
    size_t chunk = 0;

    while((chunk = stream->Read(tmp, sizeof(tmp)).LastRead()))
        buffer.Append(tmp, chunk);

    data = buffer.Data();
    len = buffer.Length();
    Ready();
}


LoaderBase* FileManager::Load(const wxString& file, bool reuseEditors)
{
    if(reuseEditors)
    {
        EditorManager* em = Manager::Get()->GetEditorManager();
        if(em)
        {
            wxFileName fileName(file);
            for(int i = 0; i < em->GetEditorsCount(); ++i)
            {
                cbEditor* ed = em->GetBuiltinEditor(em->GetEditor(i));
                if(ed && fileName == ed->GetFilename())
                {
                    wxString s(ed->GetControl()->GetText());
                    NullLoader *nl = new NullLoader(file, (char*) s.c_str(), s.length() * sizeof(wxChar));
                    return nl;
                }
            }
        }
    }

    if(file.StartsWith(_T("http://")))
    {
        URLLoader *ul = new URLLoader(file);
        urlLoaderThread.Queue(ul);
        return ul;
    }

    FileLoader *fl = new FileLoader(file);

    if(file.length() > 2 && file[0] == _T('\\') && file[1] == _T('\\'))
    {
        // UNC files behave like "normal" files, but since we know they are served over the network,
        // we can run them independently from local filesystem files for higher concurrency
        uncLoaderThread.Queue(fl);
        return fl;
    }

    fileLoaderThread.Queue(fl);
    return fl;
}



inline bool WriteWxStringToFile(wxFile& f, const wxString& data, wxFontEncoding encoding, bool bom)
{
    char* mark = NULL;
    size_t mark_length = 0;
    if (bom)
    {
        switch (encoding)
        {
        case wxFONTENCODING_UTF8:
            mark = "\xEF\xBB\xBF";
            mark_length = 3;
            break;
        case wxFONTENCODING_UTF16BE:
            mark = "\xFE\xFF";
            mark_length = 2;
            break;
        case wxFONTENCODING_UTF16LE:
            mark = "\xFF\xFE";
            mark_length = 2;
            break;
        case wxFONTENCODING_UTF32BE:
            mark = "\x00\x00\xFE\xFF";
            mark_length = 4;
            break;
        case wxFONTENCODING_UTF32LE:
            mark = "\xFF\xFE\x00\x00";
            mark_length = 4;
            break;
        case wxFONTENCODING_SYSTEM:
        default:
            break;
        }
    }
    return(f.Write(mark, mark_length) == mark_length && f.Write(data, encoding));
};

bool FileManager::Save(const wxString& name, const char* data, size_t len)
{
    if(wxFileExists(name) == false) // why bother if we don't need to
    {
        wxFile f(name, wxFile::write);
        if(!f.IsOpened())
            return false;
        return f.Write(data, len);
    }

    wxString tempName(name + _T(".cbTemp"));
    wxString backupName(name + _T(".cbBack"));

    wxFile f(tempName, wxFile::write);
    if(!f.IsOpened())
        return false;

    if(f.Write(data, len) != len)
        {
        f.Close();
        wxRemoveFile(tempName);
        return false;
        }

    if(wxRenameFile(name, backupName))
    {
        if(wxRenameFile(tempName, name))
        {
            delayedDeleteThread.Queue(new DelayedDelete(backupName));
            return true;
        }
        else
        {
            wxRenameFile(backupName, name); // restore previous state
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool FileManager::Save(const wxString& name, const wxString& data, wxFontEncoding encoding, bool bom)
{
    if(wxFileExists(name) == false) // why bother if we don't need to
    {
        wxFile f(name, wxFile::write);
        if(!f.IsOpened())
            return false;
        return WriteWxStringToFile(f, data, encoding, bom);
    }

    wxString tempName(name + _T(".cbTemp"));
    wxString backupName(name + _T(".cbBack"));

    wxFile f(tempName, wxFile::write);
    if(!f.IsOpened())
        return false;

    if(WriteWxStringToFile(f, data, encoding, bom) == false)
        {
        f.Close();
        wxRemoveFile(tempName);
        return false;
        }

    if(wxRenameFile(name, backupName))
    {
        if(wxRenameFile(tempName, name))
        {
            delayedDeleteThread.Queue(new DelayedDelete(backupName));
            return true;
        }
        else
        {
            wxRenameFile(backupName, name); // restore previous state
            return false;
        }
    }
    else
    {
        return false;
    }
}


