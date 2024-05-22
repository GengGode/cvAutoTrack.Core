#ifndef __CV_AUTOTRACK_CORE_H__
#define __CV_AUTOTRACK_CORE_H__

#if defined(_WIN32) || defined(_WIN64) || defined(_WIN128) || defined(__CYGWIN__)
    #ifdef CVAUTOTRACK_CORE_EXPORTS
        #define CVAUTOTRACK_CORE_PORT __declspec(dllexport)
    #else
        #define CVAUTOTRACK_CORE_PORT __declspec(dllimport)
    #endif
    #define CVAUTOTRACK_CORE_API CVAUTOTRACK_CORE_PORT
#elif __GNUC__ >= 4
    #define CVAUTOTRACK_CORE_API __attribute__((visibility("default")))
#else
    #define CVAUTOTRACK_CORE_API
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    typedef const char* in_string_ptr;
    typedef char* out_string_ptr;

    typedef void (*callback_t)(in_string_ptr key, in_string_ptr value, void* user_data);

    struct cvAutoTrackCore;
    typedef struct cvAutoTrackCore * core_ptr;
    struct cvAutoTrackCore
    {
        void (*destroy)(core_ptr core);
        int (*init)();
        int (*release)();
        int (*start)();
        int (*stop)();
        int (*setConfig)(in_string_ptr key, in_string_ptr value);
        int (*getConfig)(in_string_ptr key, out_string_ptr value, int size);
        int (*setCallback)(in_string_ptr callback_key, callback_t callback, void* user_data);
        int (*removeCallback)(in_string_ptr callback_key);
        int (*call)(in_string_ptr callback_key, in_string_ptr key, in_string_ptr value, void* user_data);
    };
     
    CVAUTOTRACK_CORE_API core_ptr CreateInstance();

#ifdef __cplusplus
}
#endif

#endif // __CV_AUTOTRACK_CORE_H__
