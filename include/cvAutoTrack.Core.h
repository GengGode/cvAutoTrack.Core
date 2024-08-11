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
    typedef int error_code_t;

    struct cvAutoTrackString;
    struct cvAutoTrackStringImpl;
    typedef struct cvAutoTrackString * string_ptr;
    typedef struct cvAutoTrackStringImpl * string_impl_ptr;

    struct cvAutoTrackErrorInfos;
    struct cvAutoTrackErrorInfosImpl;
    typedef struct cvAutoTrackErrorInfos * error_infos_ptr;
    typedef struct cvAutoTrackErrorInfosImpl * error_infos_impl_ptr;

    struct cvAutoTrackFrameSource;
    struct cvAutoTrackFrameSourceImpl;
    typedef struct cvAutoTrackFrameSource * frame_source_ptr;
    typedef struct cvAutoTrackFrameSourceImpl * frame_source_impl_ptr;

    struct cvAutoTrackLocalSource;
    struct cvAutoTrackLocalSourceImpl;
    typedef struct cvAutoTrackLocalSource * local_source_ptr;
    typedef struct cvAutoTrackLocalSourceImpl * local_source_impl_ptr;

    struct cvAutoTrackCaptureSource;
    struct cvAutoTrackCaptureSourceImpl;
    typedef struct cvAutoTrackCaptureSource * capture_source_ptr;
    typedef struct cvAutoTrackCaptureSourceImpl * capture_source_impl_ptr;

    struct cvAutoTrackFrameFactory;
    struct cvAutoTrackFrameFactoryImpl;
    typedef struct cvAutoTrackFrameFactory * frame_factory_ptr;
    typedef struct cvAutoTrackFrameFactoryImpl * frame_factory_impl_ptr;

    struct cvAutoTrackContext;
    struct cvAutoTrackContextImpl;
    typedef struct cvAutoTrackContext * context_ptr;
    typedef struct cvAutoTrackContextImpl * context_impl_ptr;

    struct cvAutoTrackCore;
    struct cvAutoTrackCoreImpl;
    typedef struct cvAutoTrackCore * core_ptr;
    typedef struct cvAutoTrackCoreImpl * core_impl_ptr;

    struct cvAutoTrackString
    {
        string_impl_ptr impl;
        void (*destroy)(string_ptr string);
        error_code_t (*set_string)(string_ptr string, in_string_ptr str);
        error_code_t (*get_string)(string_ptr string, out_string_ptr str, int size);
        int (*get_length)(string_ptr string);
    };

    struct cvAutoTrackErrorInfos
    {
        error_infos_impl_ptr impl;
        void (*destroy)(error_infos_ptr error_infos);
        error_code_t (*set_infos_encoding)(error_infos_ptr error_infos, in_string_ptr encoding);
        error_code_t (*get_infos_encoding)(error_infos_ptr error_infos, string_ptr encoding);
        error_code_t (*get_info)(error_infos_ptr error_infos, int index, string_ptr info);
        error_code_t (*get_info_raw)(error_infos_ptr error_infos, int index, string_ptr info);
        error_code_t (*get_info_count)(int* count);
    };

    struct cvAutoTrackFrameSource
    {
        frame_source_impl_ptr impl;
        void (*destroy)(frame_source_ptr frame_source);
        error_code_t (*set_interval)(frame_source_ptr frame_source, int ms_interval);
        error_code_t (*get_frame)(frame_source_ptr frame_source);
        error_code_t (*get_frame_data)(frame_source_ptr frame_source, out_string_ptr data, int size);
        error_code_t (*get_frame_data_size)(frame_source_ptr frame_source, int* size);
        error_code_t (*save_frame)(frame_source_ptr frame_source, in_string_ptr path);
    };

    enum local_source_type
    {
        video,
        picture
    };
    
    struct cvAutoTrackLoaclSource
    {
        local_source_impl_ptr impl;
        void (*destroy)(local_source_ptr local_source);
        frame_source_ptr (*cast)(local_source_ptr local_source);

        error_code_t (*set_local_path)(local_source_ptr local_source, in_string_ptr path);
    };

    enum capture_source_type
    {
        bitblt,
        window_graphics,
    };

    struct cvAutoTrackCaptureSource
    {
        capture_source_impl_ptr impl;
        void (*destroy)(capture_source_ptr capture_source);
        frame_source_ptr (*cast)(capture_source_ptr capture_source);

        error_code_t (*set_capture_handle)(capture_source_ptr capture_source, void* handle);
    };

    struct cvAutoTrackFrameFactory
    {
        frame_factory_impl_ptr impl;
        void (*destroy)(frame_factory_ptr frame_factory);

        frame_source_ptr (*create_local_source)(frame_factory_ptr frame_factory, local_source_type source_type);
        frame_source_ptr (*create_capture_source)(frame_factory_ptr frame_factory, capture_source_type source_type);
    };

    struct cvAutoTrackContext
    {
        context_impl_ptr impl;
        void (*destroy)(context_ptr context);
        
        error_code_t (*start_server)(context_ptr context);
        error_code_t (*stop_server)(context_ptr context);
        error_code_t (*set_frame_source)(context_ptr context, frame_source_ptr source);
        error_code_t (*get_property)(context_ptr context, in_string_ptr key, string_ptr value);
    };

    struct cvAutoTrackCore
    {
        core_impl_ptr impl;
        void (*destroy)(core_ptr core);

        error_infos_ptr (*create_error_infos)();
        context_ptr (*create_context)();
        frame_factory_ptr (*create_frame_factory)();

        error_code_t (*get_version)(string_ptr version);
        error_code_t (*get_compile_info)(string_ptr info);
    };
     
    CVAUTOTRACK_CORE_API core_ptr CreateInstance();

#ifdef __cplusplus
}
#endif

#endif // __CV_AUTOTRACK_CORE_H__
