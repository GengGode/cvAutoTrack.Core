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
        char encoding[256] = {0};
        error_infos->get_infos_encoding(error_infos, encoding, 256);
        printf("encoding: %s\n", encoding);
    }

    printf("system encoding: %s\n", get_system_encoding());

    {
        int count;
        error_infos->get_info_count(&count);
        printf("count: %d\n", count);
        char info[256] = {0};
        for (int i = 0; i < count; i++)
        {
            error_infos->get_info(error_infos, i, info, 256);
            printf("info: %s\n", info);
        }
    }

    error_infos->set_infos_encoding(error_infos, "gbk");

    {
        char encoding[256] = {0};
        error_infos->get_infos_encoding(error_infos, encoding, 256);
        printf("encoding: %s\n", encoding);
    }

    {
        int count;
        error_infos->get_info_count(&count);
        printf("count: %d\n", count);
        char info[256] = {0};
        for (int i = 0; i < count; i++)
        {
            error_infos->get_info(error_infos, i, info, 256);
            printf("info: %s\n", info);
        }
    }

    error_infos->destroy(error_infos);
    return 0;
}