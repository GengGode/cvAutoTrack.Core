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

#ifdef __cplusplus
extern "C"
{
#endif

    typedef const char* in_string_ptr;
    typedef char* out_string_ptr;

    struct cvAutoTrackErrorInfos;
    struct cvAutoTrackErrorInfosImpl;
    typedef struct cvAutoTrackErrorInfos * error_infos_ptr;
    typedef struct cvAutoTrackErrorInfosImpl * error_infos_impl_ptr;
    struct cvAutoTrackErrorInfos
    {
        error_infos_impl_ptr impl;
        void (*destroy)(error_infos_ptr error_infos);
        int (*set_infos_encoding)(error_infos_ptr error_infos, in_string_ptr encoding);
        int (*get_infos_encoding)(error_infos_ptr error_infos, out_string_ptr encoding, int size);
        int (*get_info)(error_infos_ptr error_infos, int index, out_string_ptr info, int size);
        int (*get_info_raw)(error_infos_ptr error_infos, int index, out_string_ptr info, int size);
        int (*get_info_count)(int* count);
    };


    //typedef void (*callback_t)(in_string_ptr key, in_string_ptr value, void* user_data);

    struct cvAutoTrackCore;
    struct cvAutoTrackCoreImpl;
    typedef struct cvAutoTrackCore * core_ptr;
    typedef struct cvAutoTrackCoreImpl * core_impl_ptr;
    struct cvAutoTrackCore
    {
        core_impl_ptr impl;
        void (*destroy)(core_ptr core);
        int (*sync_call)(in_string_ptr member_key, void* user_data);
        // thread safe
        int (*async_call)(in_string_ptr member_key, void* user_data, unsigned int* request_id);
        int (*get_request_status)(unsigned int request_id, int* status);

        error_infos_ptr (*create_error_infos)();

        //int (*init)();
        //int (*release)();
        //int (*start)();
        //int (*stop)();
        //int (*setConfig)(in_string_ptr key, in_string_ptr value);
        //int (*getConfig)(in_string_ptr key, out_string_ptr value, int size);
        //int (*setCallback)(in_string_ptr callback_key, callback_t callback, void* user_data);
        //int (*removeCallback)(in_string_ptr callback_key);
        //int (*call)(in_string_ptr callback_key, in_string_ptr key, in_string_ptr value, void* user_data);
    };
     
    CVAUTOTRACK_CORE_API core_ptr CreateInstance();

#ifdef __cplusplus
}
#endif

#endif // __CV_AUTOTRACK_CORE_H__
