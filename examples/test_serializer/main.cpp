//#include <QtCore/QCoreApplication>
#include <adcontrols/descriptions.h>
#include <adcontrols/massspectrum.h>
#include <adcontrols/tableofelements.h>
#include <fstream>
//#include <boost/serialization/string.hpp>
//#include <boost/serialization/nvp.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/xml_woarchive.hpp>
//#include <boost/archive/xml_wiarchive.hpp>

int main()
{
    using namespace adcontrols;

	TableOfElements * pElements = TableOfElements::instance();
	if ( pElements ) {
		std::wofstream o("elements.xml");
		o << pElements->saveXml();
		o.close();
	}

    MassSpectrum ms;
    double mass[10];
    double ints[10];
    for ( int i = 0; i < 10; ++i ) {
        mass[i] = 100 + 1.0 / 3;
        if ( i == 0 )
            ints[i] = 10;
        else
            ints[i] = ints[i-1] * ints[i-1] + mass[i];
    }

     
     ms.addDescription( Description ( L"Text A", L"key" ) );
     ms.addDescription( Description ( L"Text A", L"key" ) );
     ms.addDescription( Description ( L"Text A", L"key" ) );
    std::wofstream of("output.xml");
    ms.resize( 10 );
    ms.setMassArray( mass );
    ms.setIntensityArray(  ints );
    of << ms.saveXml();
    of.close();

}
