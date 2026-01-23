#pragma once

#ifdef _WIN32
    #ifdef MERLIN_EXPORT_DLL
        #define MERLIN_API __declspec(dllexport)
    #else
        #define MERLIN_API __declspec(dllimport)
    #endif
#else
    #define MERLIN_API
#endif

namespace Merlin {
    class MERLIN_API Engine {
    public:
        void Run();
    };
};