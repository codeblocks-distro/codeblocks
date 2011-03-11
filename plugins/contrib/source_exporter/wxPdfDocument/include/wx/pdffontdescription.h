///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontdescription.h
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2008-08-10
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontdescription.h Interface of the wxPdfFontDescription class

#ifndef _PDF_FONT_DESCRIPTION_H_
#define _PDF_FONT_DESCRIPTION_H_

// wxWidgets headers
#include <wx/string.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"

/// Class representing a font description.
class WXDLLIMPEXP_PDFDOC wxPdfFontDescription
{
public:
  /// Default constructor
  wxPdfFontDescription();
  
  /// Constructor
  /**
  * \param ascent ascender
  * \param descent descender
  * \param capHeight height of capital characters
  * \param flags font flags
  * \param fontBBox bounding box of the font
  * \param italicAngle italic angle
  * \param stemV stemV value
  * \param missingWidth width of missing characters
  * \param xHeight height of character X
  * \param underlinePosition position of underline decoration
  * \param underlineThickness thickness of underline decoration
  */
  wxPdfFontDescription(int ascent, int descent, int capHeight, int flags,
                       const wxString& fontBBox, int italicAngle, int stemV,
                       int missingWidth, int xHeight,
                       int underlinePosition, int underlineThickness);

  /// Copy constructor
  wxPdfFontDescription(const wxPdfFontDescription& desc);
  
  /// Default destructor
  ~wxPdfFontDescription();

  /// Set ascender
  /**
  * \param ascent ascender
  */
  void SetAscent(int ascent) { m_ascent = ascent; };

  /// Get ascender
  /**
  * \return the ascender
  */
  int  GetAscent() const { return m_ascent; };
  
  /// Set descender
  /**
  * \param descent descender
  */
  void SetDescent(int descent) { m_descent = descent;};

  /// Get descender
  /**
  * \return the descender
  */
  int  GetDescent() const { return m_descent;};
  
  /// Set CapHeight
  /**
  * \param capHeight the height of capital characters
  */
  void SetCapHeight(int capHeight) { m_capHeight = capHeight; };

  /// Get CapHeight
  /**
  * \return the height of capital characters
  */
  int  GetCapHeight() const { return m_capHeight; };
  
  /// Set font flags
  /**
  * \param flags the font flags
  */
  void SetFlags(int flags) { m_flags = flags; };

  /// Get font flags
  /**
  * \return the font flags
  */
  int  GetFlags() const { return m_flags; };
  
  /// Set font bounding box
  /**
  * \param fontBBox the bounding box of the font in string representation
  */
  void SetFontBBox(const wxString& fontBBox) { m_fontBBox = fontBBox; };

  /// Get font bounding box
  /**
  * \return the bounding box of the font in string representation
  */
  wxString GetFontBBox() const { return m_fontBBox; };
  
  /// Set italic angle
  /**
  * \param italicAngle the italic angle
  */
  void SetItalicAngle(int italicAngle) { m_italicAngle = italicAngle; };

  /// Get italic angle
  /**
  * \return the italic angle
  */
  int  GetItalicAngle() const { return m_italicAngle; };
  
  /// Set StemV
  /**
  * \param stemV the StemV value
  */
  void SetStemV(int stemV) { m_stemV = stemV; };

  /// Get StemV
  /**
  * \return the StemV value
  */
  int  GetStemV() const { return m_stemV; };

  /// Set missing character width
  /**
  * \param missingWidth the width of missing characters
  */
  void SetMissingWidth(int missingWidth) { m_missingWidth = missingWidth; };

  /// Get missing character width
  /**
  * \return the width of missing characters
  */
  int  GetMissingWidth() const { return m_missingWidth; };
  
  /// Set xHeight
  /**
  * \param xHeight the height of the character X
  */
  void SetXHeight(int xHeight) { m_xHeight = xHeight; };

  /// Get xHeight
  /**
  * \return the height of the character X
  */
  int  GetXHeight() const { return m_xHeight; };
  
  /// Set underline position
  /**
  * \param underlinePosition the position of the underline decoration
  */
  void SetUnderlinePosition(int underlinePosition) { m_underlinePosition = underlinePosition; };

  /// Get underline position
  /**
  * \return the position of the underline decoration
  */
  int  GetUnderlinePosition() const { return m_underlinePosition; };
  
  /// Set underline thickness
  /**
  * \param underlineThickness the thickness of the underline decoration
  */
  void SetUnderlineThickness(int underlineThickness) { m_underlineThickness = underlineThickness; };

  /// Get underline thickness
  /**
  * \return the thickness of the underline decoration
  */
  int  GetUnderlineThickness() const { return m_underlineThickness; };

private:
  int      m_ascent;                  ///< Ascender
  int      m_descent;                 ///< Descender
  int      m_capHeight;               ///< CapHeight
  int      m_flags;                   ///< Font flags
  wxString m_fontBBox;                ///< Font bounding box
  int      m_italicAngle;             ///< Angle for italics
  int      m_stemV;                   ///< StemV
  int      m_missingWidth;            ///< Missing character width
  int      m_xHeight;                 ///< xHeight
  int      m_underlinePosition;       ///< Underline position
  int      m_underlineThickness;      ///< Underline thickness
};

#endif
