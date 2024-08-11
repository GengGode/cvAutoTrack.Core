#include <Windows.h>
#include <cvAutoTrack.Core.h>
#include <stdio.h>

const char *get_system_encoding()
{
    UINT codepage = GetACP();
    switch (codepage)
    {
    case 936:
        return "gbk";
    case 65001:
        return "utf-8";
    default:
        return "unknow";
    };
}

int main()
{
    struct cvAutoTrackCore *core = CreateInstance();
    core->destroy(core);

    core = CreateInstance();

    struct cvAutoTrackErrorInfos *error_infos = core->create_error_infos();

    {
        string_ptr encoding;
        error_infos->get_infos_encoding(error_infos, &encoding);
        char encoding_str[256] = {0};
        encoding->get_string(encoding, encoding_str, 256);
        encoding->destroy(encoding);
        printf("encoding: %s\n", encoding_str);
    }

    printf("system encoding: %s\n", get_system_encoding());

    {
        int count;
        error_infos->get_info_count(&count);
        printf("count: %d\n", count);
        char info_str[256] = {0};
        for (int i = 0; i < count; i++)
        {
            string_ptr info;
            error_infos->get_info(error_infos, i, &info);
            info->get_string(info, info_str, 256); 
            info->destroy(info); 
            printf("info: %s\n", info_str);
        }
    }

    error_infos->set_infos_encoding(error_infos, "gbk");

    {
        string_ptr encoding;
        error_infos->get_infos_encoding(error_infos, &encoding);
        char encoding_str[256] = {0};
        encoding->get_string(encoding, encoding_str, 256);
        encoding->destroy(encoding);
        printf("encoding: %s\n", encoding_str);
    }

    {
        int count;
        error_infos->get_info_count(&count);
        printf("count: %d\n", count);
        char info_str[256] = {0};
        for (int i = 0; i < count; i++)
        {
            string_ptr info;
            error_infos->get_info(error_infos, i, &info);
            info->get_string(info, info_str, 256); 
            info->destroy(info); 
            printf("info: %s\n", info_str);
        }
    }

    error_infos->destroy(error_infos);
    return 0;
}