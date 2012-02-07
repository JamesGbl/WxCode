/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: version.h,v 1.9 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/

/***************************************************************************

 Licence:     wxWidgets licence

 This file has been copied from the project Mimetic
 (http://codesink.org/mimetic_mime_library.html) and relicenced from the MIT
 licence to the wxWidgets one with authorisation received from Stefano Barbato

 ***************************************************************************/

#ifndef _MIMETIC_VERSION_H_
#define _MIMETIC_VERSION_H_

#include <string>
#include <iostream>

#define MIMETC_VERSION "1.0"

namespace mimetic
{
struct Version;


// library version
extern const Version version;


// major & minor are macro defined in /usr/include/sys/sysmacros.h (linux)
// so we'll use maj & min instead

/// A three levels version string class
/**
    format:
        maj.min[.build]
          \d+\.\d+(\.\d+)?
    es. 1.1, 1.23.5, 1.2.3, 1.2.3, 1.11
        22.1.3, 0.1.234
*/
struct Version
{
    typedef unsigned int ver_type;
    Version();
    Version(const std::string&);
    Version(ver_type, ver_type, ver_type build = 0);
    void maj(ver_type);
    void v_min(ver_type);
    void v_build(ver_type);
    ver_type maj() const;
    ver_type v_min() const;
    ver_type v_build() const;

    void set(ver_type, ver_type, ver_type build = 0);
    void set(const std::string&);
    std::string str() const;

    bool operator==(const Version&) const;
    bool operator!=(const Version&) const;
    bool operator<(const Version&) const;
    bool operator>(const Version&) const;
    bool operator<=(const Version&) const;
    bool operator>=(const Version&) const;
    friend std::ostream& operator<<(std::ostream&, const Version&);
protected:
    ver_type m_maj, m_min, m_build;
};

}

#endif

