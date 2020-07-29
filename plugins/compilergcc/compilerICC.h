/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef COMPILER_ICC_H
#define COMPILER_ICC_H

#include <wx/dir.h>
#include <wx/arrstr.h>

#include <globals.h>
#include <compiler.h>

class CompilerICC : public Compiler
{
    public:
        CompilerICC();
        ~CompilerICC() override;
        AutoDetectResult AutoDetectInstallationDir() override;
    protected:
        Compiler* CreateCopy() override;
    private:
};

#endif // COMPILER_ICC_H
