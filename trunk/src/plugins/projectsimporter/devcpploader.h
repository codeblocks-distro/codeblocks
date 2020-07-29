/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef DEVCPPLOADER_H
#define DEVCPPLOADER_H

#include "ibaseloader.h"

// forward decls
class cbProject;

class DevCppLoader : public IBaseLoader
{
	public:
		DevCppLoader(cbProject* project);
		~DevCppLoader() override;

		bool Open(const wxString& filename) override;
		bool Save(const wxString& filename) override;
	protected:
        cbProject* m_pProject;
	private:
        DevCppLoader(){} // no default ctor
};

#endif // DEVCPPLOADER_H

