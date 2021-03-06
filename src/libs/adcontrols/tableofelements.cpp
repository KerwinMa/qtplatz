// -*- C++ -*-
/**************************************************************************
 ** Copyright (C) 2010-2013 Toshinobu Hondo, Ph.D.
 ** Copyright (C) 2013 MS-Cheminformatics LLC
 *
 ** Contact: info@ms-cheminfo.com
 **
 ** Commercial Usage
 **
 ** Licensees holding valid MS-Cheminformatics commercial licenses may use this file in
 ** accordance with the MS-Cheminformatics Commercial License Agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and MS-Cheminformatics.
 **
 ** GNU Lesser General Public License Usage
 **
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.TXT included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 **************************************************************************/

#include <compiler/diagnostic_push.h>
#include <compiler/disable_unused_parameter.h>
#include "adcontrols.hpp"
#include "tableofelements.hpp"
#include "element.hpp"
#include <algorithm>
#include <boost/noncopyable.hpp>
#include <adportable/array_wrapper.hpp>
#include <adportable/float.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>

#include <boost/archive/xml_woarchive.hpp>
#include <boost/archive/xml_wiarchive.hpp>

#include <compiler/diagnostic_pop.h>

#include <mutex>
#include <sstream>

using namespace adcontrols;
using namespace adcontrols::internal;

namespace adcontrols {
    namespace internal {
	
        class TableOfElementsImpl : boost::noncopyable {
        public:
            TableOfElementsImpl();
            bool internalCreate();
	    
            template<typename char_type > const adcontrols::Element&
            findElement( const std::basic_string<char_type>& symbol ) const {
                auto it = std::find_if( elements_.begin(), elements_.end(), [=]( const adcontrols::Element& e ){
                        const char_type * s = symbol.c_str();
                        const wchar_t * t = e.symbol().c_str();
                        while ( *s && *t && *s == *t ) {
                            ++s;
                            ++t;
                        }
                        return *s == char_type('\0') && *t == L'\0';
                    });
                if ( it != elements_.end() )
                    return *it;
                static adcontrols::Element empty;
                return empty;                
            }
	    
        private:
            static std::mutex mutex_;
            std::vector< Element > elements_;
            std::vector< SuperAtom > superAtoms_;
	    
        private:
            friend class boost::serialization::access;
            template<class Archive> void serialize(Archive& ar, const unsigned int version) {
                (void)version;
                ar & BOOST_SERIALIZATION_NVP(elements_) 
                    & BOOST_SERIALIZATION_NVP(superAtoms_) 
                    ;
            }
        };
    }
}

BOOST_CLASS_VERSION(adcontrols::TableOfElements, 1)
BOOST_CLASS_VERSION(adcontrols::internal::TableOfElementsImpl, 1)

TableOfElements * TableOfElements::instance_ = 0;


TableOfElements::~TableOfElements()
{
    if ( instance_ ) {
        std::lock_guard< std::mutex > lock( global_mutex::mutex() );
        if ( instance_ ) {
            delete instance_;
            instance_ = 0;
        }
    }
}

TableOfElements::TableOfElements() : pImpl_(0)
{
    pImpl_ = new TableOfElementsImpl();
    pImpl_->internalCreate();
}

TableOfElements *
TableOfElements::instance()
{
    if ( instance_ == 0 ) {
		std::lock_guard< std::mutex > lock( adcontrols::global_mutex::mutex() );
        if ( instance_ == 0 )
            instance_ = new TableOfElements();
    }
    return instance_;
}

const adcontrols::Element&
TableOfElements::findElement( const std::wstring& symbol ) const
{
    return pImpl_->findElement<wchar_t>( symbol );
}

const adcontrols::Element&
TableOfElements::findElement( const std::string& symbol ) const
{
    return pImpl_->findElement<char>( symbol );
}

//static
double
TableOfElements::getMonoIsotopicMass( const Element& e, size_t isotope )
{
    if ( isotope == 0 ) {
        // find most abundant 
        auto it = std::max_element( e.begin(), e.end(), []( const Element::Isotope&a, const Element::Isotope& b ){
                return a.abundance_ < b.abundance_;
            });
        if ( it != e.end() )
            return it->mass_;

    } else {
        // find specific isotope
        auto it = std::find_if( e.begin(), e.end(), [=]( const Element::Isotope&a ){
                return isotope == size_t( a.mass_ + 0.3 );
            });
        if ( it != e.end() )
            return it->mass_;
    }
    
    return 0;
}

//static
double
TableOfElements::getChemicalMass( const Element& e )
{
    adcontrols::Element::vector_type::const_iterator it;
    double ratio = 0;
    double mass = 0;
    
    for ( it = e.begin(); it != e.end(); ++it ) {
        ratio += it->abundance_;
        mass += it->mass_ * it->abundance_;
    }

    if ( adportable::compare<double>::approximatelyEqual( ratio, 1.0 ) )
        return mass;

    assert( adportable::compare<double>::approximatelyEqual( ratio, 1.0 ) );

    return 0;
}

std::wstring
TableOfElements::saveXml() const
{
    std::wostringstream o;
    boost::archive::xml_woarchive ar( o );
    ar << boost::serialization::make_nvp("TableOfElements", pImpl_);
    return o.str();
}

void
TableOfElements::loadXml( const std::wstring& xml )
{
	std::wistringstream in( xml );
	boost::archive::xml_wiarchive ar( in );
	ar >> boost::serialization::make_nvp("TableOfElements", pImpl_);
}

namespace adcontrols {
    namespace internal {
	
        static struct element {
            const wchar_t * symbol_;
            const wchar_t * name_;
            int atomicNumber_;
            int valence_;
            int isotopeCount_;
            struct { double mass_; double abund_; } ma_[10];
        } __elementTable__ [] = {
            // Symbol Name AtomicNumber Valence NumberOfIsotope Weight_1      Ratio_1...
            { L"H",   L"Hydrogen",       1,   1,  2,  { { 1.007825,     0.999885 }, 
                                                        { 2.0141018,    0.000115   } } },
            { L"He",  L"Helium",         2,   0,  2,  { { 3.0160293,    0.00000137 },  
                                                        { 4.0026033,    0.99999863 } } },
            { L"Li",  L"Lithium",        3,   1,  2,  { { 6.0151223,    0.0759 },      
                                                        { 7.0160041,    0.9241 } } },
            { L"Be",  L"Beryllium",      4,   2,  1,  { { 9.0121822,    1 } } },
            { L"B",   L"Boron",          5,   3,  2,  { { 10.0129371,   0.199, },       
                                                        { 11.0093055,   0.801 } } },
            { L"C",   L"Carbon",         6,   4,  2,  { { 12,           0.9893 },      
                                                        { 13.0033548,   0.0107 } } },
            { L"N",   L"Nitrogen",       7,   3,  2,  { { 14.003074,    0.99632 },     
                                                        { 15.000109,    0.00368 } } },
            { L"O",   L"Oxygen",         8,   2,  3,  { { 15.9949146,   0.99757 },     
                                                        { 16.9991315,   0.00038 },     
                                                        { 17.9991604,   0.00205 } } },
            { L"F",   L"Fluorine",       9,   1,  1,  { { 18.9984032,   1 } } },
            { L"Ne",  L"Neon",           10,  0,  3,  { { 19.9924402,   0.9048 },      
                                                        { 20.9938467,   0.0027 },      
                                                        { 21.9913855,   0.0925 } } },
            { L"Na",  L"Sodium",         11,  1,  1,  { { 22.9897697,   1 } } },
            { L"Mg",  L"Magnesium",      12,  2,  3,  { { 23.9850419,   0.7899 },      
                                                        { 24.985837,    0.1 },         
                                                        { 25.982593,    0.1101 } } },
            { L"Al",  L"Aluminium",      13,  3,  1,  { { 26.9815384,   1 } } },
            { L"Si",  L"Silicon",        14,  4,  3,  { { 27.9769265,   0.922297 },    
                                                        { 28.9764947,   0.046832 },
                                                        { 29.9737702,   0.030872 } } },
            { L"P",   L"Phosphorus",     15,  5,  1,  { { 30.9737615,   1 } } },
            { L"S",   L"Sulfur",         16,  4,  4,  { { 31.9720707,   0.9493 },      
                                                        { 32.9714585,   0.0076 },      
                                                        { 33.9678669,   0.0429 },      
                                                        { 35.9670809,   0.0002 } } },
            { L"Cl",  L"Chlorine",       17,  1,  2,  { { 34.9688527,   0.7578 },      
                                                        { 36.9659026,   0.2422 } } },
            { L"Ar",  L"Argon",          18,  0,  3,  { { 35.9675463,   0.003365 },    
                                                        { 37.9627322,   0.000632 },    
                                                        { 39.9623831,   0.996003 } } },
            { L"K",   L"Potassium",      19,  1,  3,  { { 38.9637069,   0.932581 },    
                                                        { 39.9639987,   0.000117 },    
                                                        { 40.961826,    0.067302 } } },
            { L"Ca",  L"Calcium",        20,  2,  6,  { { 39.9625912,   0.96941 },     
                                                        { 41.9586183,   0.00647 },     
                                                        { 42.9587668,   0.00135 },     
                                                        { 43.9554811,   0.02086 },     
                                                        { 45.9536927,   0.00004 },     
                                                        { 47.9525334,   0.00187 } } },
            { L"Sc",  L"Scandium",       21,  3,  1,  { { 44.9559102,   1 } } },
            { L"Ti",  L"Titanium",       22,  2,  5,  { { 45.9526295,   0.0825 },      
                                                        { 46.9517637,   0.0744 },      
                                                        { 47.947947,    0.7372 },      
                                                        { 48.9478707,   0.0541 },      
                                                        { 49.944792,    0.0518 } } },
            { L"V",   L"Vanadium",       23,  3,  2,  { { 49.9471627,   0.0025 },      
                                                        { 50.9439635,   0.9975 } } },
            { L"Cr",  L"Chromium",       24,  3,  4,  { { 49.9460495,   0.04345 },     
                                                        { 51.9405115,   0.83789 },     
                                                        { 52.9406534,   0.09501 },     
                                                        { 53.9388846,   0.02365 } } },
            { L"Mn",  L"Manganese",      25,  4,  1,  { { 54.9380493,   1 } } },
            { L"Fe",  L"Iron",           26,  3,  4,  { { 53.9396147,   0.05845 },     
                                                        { 55.9349418,   0.91754 },     
                                                        { 56.9353983,   0.02119 },     
                                                        { 57.9332801,   0.00282 } } },
            { L"Co",  L"Cobalt",         27,  3,  1,  { { 58.9331999,   1 } } },
            { L"Ni",  L"Nickel",         28,  3,  5,  { { 57.9353477,   0.680769 },    
                                                        { 59.9307903,   0.262231 },    
                                                        { 60.9310601,   0.011399 },    
                                                        { 61.9283484,   0.036345 },    
                                                        { 63.9279692,   0.009256 } } },
            { L"Cu",  L"Copper",         29,  2,  2,  { { 62.9296007,   0.6917 },      
                                                        { 64.9277938,   0.3083 } } },
            { L"Zn",  L"Zinc",           30,  2,  5,  { { 63.9291461,   0.4863 },      
                                                        { 65.9260364,   0.279 },       
                                                        { 66.9271305,   0.041 },       
                                                        { 67.9248473,   0.1875 },      
                                                        { 69.9253246,   0.0062 } } },
            { L"Ga",  L"Gallium",        31,  3,  2,  { { 68.9255807,   0.60108 },     
                                                        { 70.9247074,   0.39892 } } },
            { L"Ge",  L"Germanium",      32,  4,  5,  { { 69.92425,     0.2084 },      
                                                        { 71.9220763,   0.2754 },      
                                                        { 72.9234595,   0.0773 },      
                                                        { 73.9211784,   0.3628 },      
                                                        { 75.9214029,   0.0761 } } },
            { L"As",  L"Arsenic",        33,  3,  1,  { { 74.9215966,   1 } } },
            { L"Se",  L"Selenium",       34,  4,  6,  { { 73.9224767,   0.0089 },      
                                                        { 75.9192143,   0.0937 },      
                                                        { 76.9199148,   0.0763 },      
                                                        { 77.9173097,   0.2377 },      
                                                        { 79.9165221,   0.4961 },      
                                                        { 81.9167003,   0.0873 } } },
            { L"Br",  L"Bromine",        35,  1,  2,  { { 78.9183379,   0.5069 },      
                                                        { 80.9162914,   0.4931 } } },
            { L"Kr",  L"Krypton",        36,  0,  6,  { { 77.9203879,   0.0035 },      
                                                        { 79.9163785,   0.0228 },      
                                                        { 81.913485,    0.1158 },      
                                                        { 82.9141372,   0.1149 },      
                                                        { 83.9115081,   0.57 },        
                                                        { 85.9106151,   0.173 } } },
            { L"Rb",  L"Rubidium",       37,  1,  2,  { { 84.9117924,   0.7217 },      
                                                        { 86.9091858,   0.2783 } } },
            { L"Sr",  L"Strontium",      38,  2,  4,  { { 83.9134265,   0.0056 },      
                                                        { 85.9092647,   0.0986 },      
                                                        { 86.9088816,   0.07 },        
                                                        { 87.9056167,   0.8258 } } },
            { L"Y",   L"Yttrium",        39,  3,  1,  { { 88.9058485,   1 } } },
            { L"Zr",  L"Zirconium",      40,  4,  5,  { { 89.9047022,   0.5145 },      
                                                        { 90.9056434,   0.1122 },      
                                                        { 91.9050386,   0.1715 },      
                                                        { 93.9063144,   0.1738 },      
                                                        { 95.9082747,   0.028 } } },
            { L"Nb",  L"Niobium",        41,  3,  1,  { { 92.9063762,   1 } } },
            { L"Mo",  L"Molybdenum",     42,  3,  7,  { { 91.9068095,   0.1484 } ,      
                                                        { 93.9050867,   0.0925 },      
                                                        { 94.9058406,   0.1592 },      
                                                        { 95.904678,    0.1668 },      
                                                        { 96.9060201,   0.0955 },      
                                                        { 97.9054069,   0.2413 },      
                                                        { 99.9074765,   0.0963 } } },
            { L"Tc", L"Technetium",      43,  2,   1, { { 96.906370000,  1.0000 } } },
            { L"Ru",  L"Ruthenium",      44,  3,  7,  { { 95.9076036,   0.0554 },      
                                                        { 97.905287,    0.0187 },      
                                                        { 98.9059385,   0.1276 },      
                                                        { 99.9042189,   0.126 },       
                                                        { 100.9055815,  0.1706 },      
                                                        { 101.9043488,  0.3155 },      
                                                        { 103.9054295,  0.1862 } } },
            { L"Rh",  L"Rhodium",        45,  3,  1,  { { 102.9055036,  1 } } },
            { L"Pd",  L"Palladium",      46,  2,  6,  { { 101.9056071,  0.0102 },      
                                                        { 103.9040342,  0.1114 },      
                                                        { 104.9050833,  0.2233 },      
                                                        { 105.9034835,  0.2733 },      
                                                        { 107.9038948,  0.2646 },      
                                                        { 109.9051527,  0.1172 } } },
            { L"Ag",  L"Silver",         47,  1,  2,  { { 106.9050931,  0.51839 },     
                                                        { 108.9047559,  0.48161 } } },
            { L"Cd",  L"Cadmium",        48,  2,  8,  { { 105.906458,   0.0125 },      
                                                        { 107.9041831,  0.0089 },      
                                                        { 109.903006,   0.1249 },      
                                                        { 110.904182,   0.128 },       
                                                        { 111.9027577,  0.2413 },      
                                                        { 112.9044014,  0.1222 },      
                                                        { 113.9033586,  0.2873 },      
                                                        { 115.9047558,  0.0749 } } },
            { L"In",  L"Indium",         49,  3,  2,  { { 112.9040623,  0.0429 },      
                                                        { 114.903879,   0.9571 } } },
            { L"Sn",  L"Tin",            50,  2,  10, { { 111.9048218,  0.0097 },      
                                                        { 113.902783,   0.0066 },      
                                                        { 114.9033473,  0.0034 },      
                                                        { 115.9017455,  0.1454 },      
                                                        { 116.9029551,  0.0768 },      
                                                        { 117.9016079,  0.2422 },      
                                                        { 118.9033106,  0.0859 },      
                                                        { 119.9021985,  0.3258 },      
                                                        { 121.9034411,  0.0463 },      
                                                        { 123.9052745,  0.0579 } } },
            { L"Sb",  L"Antimony",       51,  3,  2,  { { 120.9038222,  0.5721 },      
                                                        { 122.904216,   0.4279 } } },
            { L"Te",  L"Tellurium",      52,  4,  8,  { { 119.9040264,  0.0009 },      
                                                        { 121.9030558,  0.0255 },      
                                                        { 122.9042711,  0.0089 },      
                                                        { 123.9028188,  0.0474 },      
                                                        { 124.9044241,  0.0707 },      
                                                        { 125.9033049,  0.1884 },      
                                                        { 127.9044615,  0.3174 },      
                                                        { 129.9062229,  0.3408 } } },
            { L"I",   L"Iodine",         53,  1,  1,  { { 126.9044677,  1 } } },
            { L"Xe",  L"Xenon",          54,  0,  9,  { { 123.9058954,  0.0009 },      
                                                        { 125.9042682,  0.0009 },      
                                                        { 127.9035305,  0.0192 },      
                                                        { 128.9047799,  0.2644 },      
                                                        { 129.9035089,  0.0408 },      
                                                        { 130.9050828,  0.2118 },      
                                                        { 131.9041546,  0.2689 },      
                                                        { 133.9053945,  0.1044 },      
                                                        { 135.9072199,  0.0887 } } },
            { L"Cs",  L"Caesium",        55,  0,  1,  { { 132.9054472,  1 } } },
            { L"Ba",  L"Barium",         56,  2,  7,  { { 129.9063109,  0.00106 },     
                                                        { 131.9050564,  0.00101 },     
                                                        { 133.9045036,  0.02417 },     
                                                        { 134.9056838,  0.06592 },     
                                                        { 135.9045712,  0.07854 },     
                                                        { 136.9058225,  0.11232 },     
                                                        { 137.9052423,  0.71698 } } },
            { L"La",  L"Lanthanum",      57,  3,  2,  { { 137.9071076,  0.0009 },      
                                                        { 138.9063489,  0.9991 } } },
            { L"Ce",  L"Cerium",         58,  3,  4,  { { 135.9071443,  0.00185 },     
                                                        { 137.9059863,  0.00251 },     
                                                        { 139.9054348,  0.8845 },      
                                                        { 141.9092406,  0.11114 } } },
            { L"Pr",  L"Praseodymium",   59,  3,  1,  { { 140.9076484,  1 } } },
            { L"Nd",  L"Neodymium",      60,  3,  7,  { { 141.9077193,  0.272 },
                                                        { 142.9098102,  0.122 },
                                                        { 143.9100832,  0.238 },
                                                        { 144.9125694,  0.083 },
                                                        { 145.9131127,  0.172 },
                                                        { 147.9168891,  0.057 },
                                                        { 149.920887,   0.056 } } },
	    
            { L"Pm",  L"Promethium",        61,  3,   1,  { { 145.000000000,  1.0000 } } },
            { L"Sm",  L"Samarium",       62,  3,  7,  { { 143.9119958,  0.0307 },
                                                        { 146.9148938,  0.1499 },
                                                        { 147.9148184,  0.1124 },
                                                        { 148.9171799,  0.1382 },
                                                        { 149.9172718,  0.0738 },
                                                        { 151.9197288,  0.2675 },
                                                        { 153.9222058,  0.2275 } } },
            { L"Eu",  L"Europium",       63,  3,  2,  { { 150.9198463,  0.4781 },
                                                        { 152.9212265,  0.5219 } } },
            { L"Gd",  L"Gadolinium",     64,  3,  7,  { { 151.9197888,  0.002 },
                                                        { 153.9208624,  0.0218 },
                                                        { 154.9226189,  0.148 },
                                                        { 155.9221197,  0.2047 },
                                                        { 156.9239568,  0.1565 },
                                                        { 157.9241006,  0.2484 },
                                                        { 159.9270507,  0.2186 } } },
            { L"Tb",  L"Terbium",        65,  3,  1,  { { 158.9253432,  1 } } },
            { L"Dy",  L"Dysprosium",     66,  3,  7,  { { 155.9242783,  0.0006 },
                                                        { 157.9244047,  0.001 },
                                                        { 159.9251938,  0.0234 },
                                                        { 160.9269297,  0.1891 },
                                                        { 161.9267948,  0.2551 },
                                                        { 162.9287276,  0.249 },
                                                        { 163.9291712,  0.2818 } } },
            { L"Ho",  L"Holmium",        67,  3,  1,  { { 164.9303191,  1 } } },
            { L"Er",  L"Erbium",         68,  3,  6,  { { 161.9287751,  0.0014 },
                                                        { 163.9291973,  0.0161 },
                                                        { 165.9302903,  0.3361 },
                                                        { 166.9320458,  0.2293 },
                                                        { 167.9323681,  0.2678 },
                                                        { 169.9354607,  0.1493 } } },
            { L"Tm",  L"Thulium",        69,  3,  1,  { { 168.9342114,  1 } } },
            { L"Yb",  L"Ytterbium",      70,  3,  7,  { { 167.9338947,  0.0013 },
                                                        { 169.9347589,  0.0304 },
                                                        { 170.9363226,  0.1428 },
                                                        { 171.936378,   0.2183 },
                                                        { 172.938207,   0.1613 },
                                                        { 173.9388584,  0.3183 },
                                                        { 175.9425687,  0.1276 } } },
            { L"Lu",  L"Lutetium",       71,  3,  2,  { { 174.9407682,  0.9741 },
                                                        { 175.9426827,  0.0259 } } },
            { L"Hf",  L"Hafnium",        72,  3,  6,  { { 173.9400415,  0.0016 },
                                                        { 175.9414033,  0.0526 },
                                                        { 176.9432204,  0.186 },
                                                        { 177.9436981,  0.2728 },
                                                        { 178.9458154,  0.1362 },
                                                        { 179.9465488,  0.3508 } } },
            { L"Ta",  L"Tantalum",       73,  5,  2,  { { 179.9474655,  0.00012 },
                                                        { 180.9479962,  0.99988 } } },
            { L"W",   L"Tungsten",       74,  4,  5,  { { 179.9467055,  0.0012 },
                                                        { 181.9482053,  0.265 },
                                                        { 182.9502242,  0.1431 },
                                                        { 183.9509323,  0.3064 },
                                                        { 185.9543619,  0.2843 } } },
            { L"Re",  L"Rhenium",        75,  3,  2,  { { 184.9529555,  0.374 },
                                                        { 186.9557505,  0.626 } } },
            { L"Os",  L"Osmium",         76,  3,  7,  { { 183.9524906,  0.0002 },
                                                        { 185.9538381,  0.0159 },
                                                        { 186.9557476,  0.0196 },
                                                        { 187.9558357,  0.1324 },
                                                        { 188.9581445,  0.1615 },
                                                        { 189.9584448,  0.2626 },
                                                        { 191.9614787,  0.4078 } } },
            { L"Ir",  L"Iridium",        77,  3,  2,  { { 190.9605908,  0.373 },
                                                        { 192.9629233,  0.627 } } },
            { L"Pt",  L"Platinum",       78,  2,  6,  { { 189.9599297,  0.00014 },
                                                        { 191.9610347,  0.00782 },
                                                        { 193.962663,   0.32967 },
                                                        { 194.9647739,  0.33832 },
                                                        { 195.9649343,  0.25242 },
                                                        { 197.9678754,  0.07163 } } },
            { L"Au",  L"Gold",           79,  1,  1,  { { 196.966551,   1 } } },
            { L"Hg",  L"Mercury",        80,  1,  7,  { { 195.9658145,  0.0015 },
                                                        { 197.9667515,  0.0997 },
                                                        { 198.9682623,  0.1687 },
                                                        { 199.9683085,  0.231 },
                                                        { 200.9702851,  0.1318 },
                                                        { 201.9706254,  0.2986 },
                                                        { 203.9734754,  0.0687 } } },
            { L"Tl",  L"Thallium",       81,  3,  2,  { { 202.9723286,  0.29524 },
                                                        { 204.9744118,  0.70476 } } },
            { L"Pb",  L"Lead",           82,  2,  4,  { { 203.9730284,  0.014 },
                                                        { 205.9744488,  0.241 },
                                                        { 206.9758804,  0.221 },
                                                        { 207.9766356,  0.524 } } },
            { L"Bi",  L"Bismuth",        83,  3,  1,   { { 208.9803836,  1 } } },
            { L"Po", L"Polonium",        84,  0,   1,  { { 209.000000000,  1.0000 } } },
            { L"At", L"Astatine",        85,  0,   1,  { { 210.000000000,  1.0000 } } },
            { L"Rn", L"Radon",           86,  0,   1,  { { 222.000000000,  1.0000 } } },
            { L"Fr", L"Francium",        87,  0,   1,  { { 223.000000000,  1.0000 } } },
            { L"Ra", L"Radon",           88,  2,   1,  { { 226.025400000,  1.0000 } } },
            { L"Ac", L"Actinium",        89,  0,   1,  { { 227.000000000,  1.0000 } } },
            { L"Th",  L"Thorium",        90,  4,  1,  { { 232.0380495,  1 } } },
            { L"Pa",  L"Protactinium",   91,  0,  1,  { { 231.035878,   1 } } },
            { L"U",   L"Uranium",        92,  4,  3,  { { 234.0409447,  0.000055 },
                                                        { 235.0439222,  0.0072 },
                                                        { 238.0507835,  0.992745 } } },
            { L"Np", L"Neptunium",       93,  3,   1,  { { 237.048200000,  1.0000 } } },
            { L"Pu", L"Plutonium",       94,  0,   1,  { { 244.000000000,  1.0000 } } },
            { L"Am", L"Americium",       95,  0,   1,  { { 243.000000000,  1.0000 } } },
            { L"Cm", L"Curium",          96,  0,   1,  { { 247.000000000,  1.0000 } } },
            { L"Bk", L"Berkelium",       97,  0,   1,  { { 247.000000000,  1.0000 } } },
            { L"Cf", L"Californium",     98,  0,   1,  { { 251.000000000,  1.0000 } } },
            { L"Es", L"Einsteinium",     99,  0,   1,  { { 254.000000000,  1.0000 } } },
            { L"Fm", L"Fermium",        100,  0,   1,  { { 257.000000000,  1.0000 } } },
            { L"Md", L"Mendelevium",    101,  0,   1,  { { 258.000000000,  1.0000 } } },
            { L"No", L"Nobelium",       102,  0,   1,  { { 259.000000000,  1.0000 } } },
            { L"Lr", L"Lawrencium",     103,  0,   1,  { { 260.000000000,  1.0000 } } },
        };
	
        static struct superatom {
            const wchar_t * name_;
            const wchar_t * alias_;
            const wchar_t * formula_;
            int valence_;
        } __superAtoms__[] = {
            // ;AminoAcidL"Alias", Formula, Valence },
            { L"A", L"Ala",           L"C3H5NO",               2 },
            { L"R", L"Arg",           L"C6H12N4O",             2 },
            { L"N", L"Asn",           L"C4H6N2O2",             2 },
            { L"D", L"Asp",           L"C4H5NO3",              2 },
            { L"C", L"Cys",           L"C3H5NOS",              2 },
            { L"Q", L"Gln",           L"C5H8N2O2",             2 },
            { L"E", L"Glu",           L"C5H7NO3",              2 },
            { L"G", L"Gly",           L"C2H3NO",               2 },
            { L"H", L"His",           L"C6H7N3O",              2 },
            { L"I", L"Ile",           L"C6H11NO",              2 },
            { L"L", L"Leu",           L"C6H11NO",              2 },
            { L"K", L"Lys",           L"C6H12N2O",             2 },
            { L"M", L"Met",           L"C5H9NOS",              2 },
            { L"F", L"Phe",           L"C9H9NO",               2 },
            { L"P", L"Pro",           L"C5H7NO",               2 },
            { L"S", L"Ser",           L"C3H5NO2",              2 },
            { L"T", L"Thr",           L"C4H7NO2",              2 },
            { L"W", L"Trp",           L"C11H10N2O",            2 },
            { L"Y", L"Tyr",           L"C9H9NO2",              2 },
            { L"V", L"Val",           L"C5H9NO",               2 },
        };
	
        /*
          [DNA] },
          ; },
          ;SymbolL"Alias", Formula, Valence },
          ; },
          { L"A", L"Adenine",       C10H12N5O5P,          2 },
          { L"C", L"Cytosine",      C9H12N3O6P,           2 },
          { L"G", L"Guanine",       C10H12N5O6P,          2 },
          { L"T", L"Thymine",       C10H13N2O7P,          2 },
          },
          [RNA] },
          ; },
          ;SymbolL"Alias", Formula, Valence },
          ; },
          { L"A", L"Adenine",       C10H12N5O6P,          2 },
          { L"C", L"Cytosine",      C9H12N3O7P,           2 },
          { L"G", L"Guanine",       C10H12N5O7P,          2 },
          { L"U", L"Uracil",        C9H11N2O8P,           2 },
        */

    } // namespace internal
} // namespace adcontrols

///////////////////////////////

namespace adcontrols {
    namespace internal {

        struct set_element_vec {

            std::vector< Element >& vec_;
            set_element_vec( std::vector<Element>& v ) : vec_(v) {}

            void operator()( const element& e ) {
				Element elm( e.symbol_, e.name_, e.atomicNumber_, e.valence_ );
                for ( int i = 0; i < e.isotopeCount_; ++i )
                    elm.addIsotope( Element::Isotope( e.ma_[i].mass_, e.ma_[i].abund_ ) );
                vec_.push_back( elm );
            }
        };

        struct set_superatom_vec {
            std::vector< SuperAtom >& vec_;
            set_superatom_vec( std::vector<SuperAtom>& v ) : vec_(v) {}
            void operator()( const superatom& a ) {
                vec_.push_back( SuperAtom( a.name_, a.alias_, a.formula_, a.valence_ ) );
            }
        };

    }
}

////////////////////////

TableOfElementsImpl::TableOfElementsImpl()
{
}

bool
TableOfElementsImpl::internalCreate() 
{

#define countof(array) (sizeof(array)/sizeof(array)[0])

    using namespace adportable;

    /* elements */
    array_wrapper<const element> elements( __elementTable__, countof(__elementTable__));

    // std::for_each( elements.begin(), elements.end(), set_element(pi_) );
    std::for_each( elements.begin(), elements.end(), set_element_vec( this->elements_ ) );

    /* super atoms */
    array_wrapper<const superatom> atoms( __superAtoms__, countof(__superAtoms__) );

    // std::for_each( atoms.begin(), atoms.end(), set_superatom(pi_) );
    std::for_each( atoms.begin(), atoms.end(), set_superatom_vec( this->superAtoms_ ) );

    return true;
}

