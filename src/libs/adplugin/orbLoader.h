#pragma once

#if defined(ADPLUGIN_LIBRARY)
#  define SHARED_EXPORT __declspec(dllexport)
#else
#  define SHARED_EXPORT __declspec(dllimport)
#endif

namespace CORBA {
    class ORB;
}

namespace adplugin {

    class SHARED_EXPORT orbLoader {
    public:
		virtual ~orbLoader() {};
		virtual operator bool() const = 0;

		virtual bool initialize( CORBA::ORB * orb = 0 ) = 0;
        virtual bool activate() = 0;
        virtual bool deactivate() = 0;
        virtual int run() = 0;
        virtual void abort_server() = 0;
    };

}