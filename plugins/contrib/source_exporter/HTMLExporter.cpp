/*
 * This code is based in the HTML output generated by Dev-C++
 */

#include "HTMLExporter.h"
#include <configmanager.h>
#include <wx/fontutil.h>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <fstream>

using std::ofstream;
using std::ostringstream;
using std::hex;
using std::setw;
using std::setfill;
using std::uppercase;
using std::size_t;

namespace
{
  // Helper function to convert i to a string
  inline string to_string(int i)
  {
    ostringstream ostr;

    ostr << i;

    return ostr.str();
  }
};

const char *HTMLExporter::HTMLHeaderBEG =
  "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n"
  "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
  "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
  "<head>\n";

const char *HTMLExporter::HTMLMeta =
  "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />\n"
  "<meta name=\"generator\" content=\"Code::Blocks Exporter plugin\" />\n";

const char *HTMLExporter::HTMLStyleBEG =
  "<style type=\"text/css\">\n"
  "<!--\n";

string HTMLExporter::HTMLStyle(const EditorColorSet *c_color_set, HighlightLanguage lang)
{
  string style_list;
  string style_body("body { color: #000000; background-color: #FFFFFF; }\n");
  EditorColorSet *color_set = const_cast<EditorColorSet *>(c_color_set);

  if (lang == HL_NONE)
  {
    return style_body + style_list;
  }

  const int count = color_set->GetOptionCount(lang);

  for (int i = 0; i < count; ++i)
  {
    OptionColor *optc = color_set->GetOptionByIndex(lang, i);

    if (!optc->isStyle)
    {
      continue;
    }

    ostringstream ostr;

    // If it's the style 0 add it as body, otherwise it's just a custom style
    if (optc->value == 0)
    {
      ostr << "body" << " { color: #" << hex << setfill('0') << uppercase
           << setw(2) << static_cast<unsigned int>(optc->fore.Red())
           << setw(2) << static_cast<unsigned int>(optc->fore.Green())
           << setw(2) << static_cast<unsigned int>(optc->fore.Blue())
           << "; ";
    }
    else
    {
      ostr << ".style" << optc->value << " { color: #" << hex << setfill('0') << uppercase
           << setw(2) << static_cast<unsigned int>(optc->fore.Red())
           << setw(2) << static_cast<unsigned int>(optc->fore.Green())
           << setw(2) << static_cast<unsigned int>(optc->fore.Blue())
           << "; ";
    }

    if (optc->back.Ok())
    {
      ostr << "background-color: #"
           << setw(2) << static_cast<unsigned int>(optc->back.Red())
           << setw(2) << static_cast<unsigned int>(optc->back.Green())
           << setw(2) << static_cast<unsigned int>(optc->back.Blue())
           << "; ";
    }

    if (optc->bold)
    {
      ostr << "font-weight: bold; ";
    }

    if (optc->italics)
    {
      ostr << "font-style: italic; ";
    }

    if (optc->underlined)
    {
      ostr << "text-decoration: underline; ";
    }

    ostr << "}\n";

    // Once again, if it's the style 0 then it's the body
    if (optc->value == 0)
    {
      style_body = ostr.str();
    }
    else
    {
      style_list += ostr.str();
    }
  }

  return style_body + style_list;
}

const char *HTMLExporter::HTMLStyleEND =
  "-->"
  "</style>";

const char *HTMLExporter::HTMLHeaderEND =
  "</head>\n";

const char *HTMLExporter::HTMLBodyBEG =
  "<body>\n"
  "<pre>\n";

string HTMLExporter::HTMLBody(const wxMemoryBuffer &styled_text)
{
  string html_body("<code><span style=\"font: 8pt Courier New;\">");
  const char *buffer = reinterpret_cast<char *>(styled_text.GetData());
  const size_t buffer_size = styled_text.GetDataLen();

  wxString fontstring = ConfigManager::Get()->Read(_T("/editor/font"), wxEmptyString);

  if (!fontstring.IsEmpty())
  {
    wxFont tmpFont;
    wxNativeFontInfo nfi;
    nfi.FromString(fontstring);
    tmpFont.SetNativeFontInfo(nfi);

    int pt = tmpFont.GetPointSize();
    wxString faceName = tmpFont.GetFaceName();

    if (!faceName.IsEmpty())
    {
      html_body = string("<code><span style=\"font: ") + to_string(pt) + string("pt ") + string(faceName.c_str()) + string(";\">");
    }
  }

  if (buffer_size == 0)
  {
    return html_body;
  }

  // Get the current style from the first character
  char current_style = buffer[1];

  // If the first style happen to be the body style...
  if (current_style == 0)
  {
    html_body += string("<span class=\"body\">");
  }
  else
  {
    html_body += string("<span class=\"style") + to_string(current_style) + "\">";
  }

  for (size_t i = 0; i < buffer_size; i += 2)
  {
    if (buffer[i + 1] != current_style)
    {
      if (!isspace(buffer[i]))
      {
        html_body += "</span>";
        current_style = buffer[i + 1];
        html_body += string("<span class=\"style") + to_string(current_style) + "\">";
      }
    }

    switch (buffer[i])
    {
      case '<':
        html_body += "&lt;";
        break;

      case '>':
        html_body += "&gt;";
        break;

      case '&':
        html_body += "&amp;";
        break;

      case '\r':
        break;

      default:
        html_body += buffer[i];
        break;
    }
  }

  html_body += "</span>";

  return html_body;
}

const char *HTMLExporter::HTMLBodyEND =
  "</span>\n"
  "</code></pre>\n"
  "</body>\n"
  "</html>\n";

void HTMLExporter::Export(const wxString &filename, const wxString &title, const wxMemoryBuffer &styled_text, const EditorColorSet *color_set)
{
  string html_code;
  HighlightLanguage lang = const_cast<EditorColorSet *>(color_set)->GetLanguageForFilename(title);

  html_code += HTMLHeaderBEG;
  html_code += string("<title>") + string(reinterpret_cast<const char *>(title.c_str())) + string("</title>\n");
  html_code += HTMLMeta;
  html_code += HTMLStyleBEG;
  html_code += HTMLStyle(color_set, lang);
  html_code += HTMLStyleEND;
  html_code += HTMLHeaderEND;
  html_code += HTMLBodyBEG;
  html_code += HTMLBody(styled_text);
  html_code += HTMLBodyEND;

  ofstream file(filename.c_str());
  file << html_code;
}
