#ifndef WXSWINDOWRESDATAOBJECT_H
#define WXSWINDOWRESDATAOBJECT_H

#include <wx/dataobj.h>
#include <tinyxml/tinyxml.h>

#define wxsDF_WIDGET   _T("wxSmith XML")

class wxsItem;
class wxsWindowRes;

/** \brief Class representing one or more items with resource structure using wxDataObject class */
class wxsWindowResDataObject : public wxDataObject
{
	public:

        /** \brief Ctor */
		wxsWindowResDataObject();

		/** \brief Dctor */
		virtual ~wxsWindowResDataObject();

    //=====================================
    // Opertating on data
    //=====================================

        /** \brief Clering all data */
        void Clear();

		/** \brief Adding widget into this data object */
		bool AddItem(wxsItem* Item);

		/** \brief Getting number of handled widgets inside this object */
		int GetItemCount() const;

		/** \brief Building wxsItem class from this data object
		 *  \param Resource - resource owning item
		 *  \param Index - id of item (in range 0..GetWidgetCount()-1)
		 *  \return created item or NULL on error
		 */
		wxsItem* BuildItem(wxsWindowRes* Resource,int Index = 0) const;

		/** \brief Setting Xml string describing widget */
		bool SetXmlData(const wxString& Data);

		/** \brief Getting Xml strting desecribing widget */
		wxString GetXmlData() const;

    //=====================================
    // Members of wxDataObject class
    //=====================================

		/** \brief Enumerating all data formats.
		 *
		 * Formats available for reading and writing:
         * - wxDF_TEXT
         * - internal type ("wxSmith XML")
         */
		virtual void GetAllFormats(wxDataFormat *formats,Direction dir) const;

		/** \brief Copying data to raw buffer */
		virtual bool GetDataHere(const wxDataFormat& format,void *buf) const;

		/** \brief Returns number of data bytes */
		virtual size_t GetDataSize(const wxDataFormat& format) const;

		/** \brief Returns number of suported formats (in both cases - 2) */
		virtual size_t GetFormatCount(Direction dir) const;

		/** \brief Returning best format - "wxSmith XML" */
		virtual wxDataFormat GetPreferredFormat(Direction dir) const;

		/** \brief Setting data - will load Xml data */
		virtual bool SetData(const wxDataFormat& format,size_t len,const void *buf);

    private:

        TiXmlDocument XmlDoc;
        TiXmlElement* XmlElem;
        int ItemCount;
};

#endif
