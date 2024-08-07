﻿/******************************************************************************************
*                                                                                         *
*    Notifier                                                                             *
*                                                                                         *
*    Copyright (c) 2023 Onur AKIN <https://github.com/onurae>                             *
*    Licensed under the MIT License.                                                      *
*                                                                                         *
******************************************************************************************/

#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include "imgui.h"
#include "imgui_internal.h"
#include <vector>
#include <string> 
#undef ERROR // Fix: wingdi.h define Error 0
class Notif
{
public:
    enum class Status
    {
        FADEIN,
        ON,
        FADEOUT,
        OFF
    };
    enum class Type
    {
        NONE,
        SUCCESS,
        WARNING,
        ERROR,
        INFO
    };
    Notif(Type type, const std::string& title, const std::string& content = "", float onTime = 3.0f);
    virtual ~Notif() = default;

    Status GetStatus() const;
    std::string GetIcon() const;
    std::string GetTitle() const { return title; }
    std::string GetContent() const { return content; }
    std::string GetTypeName() const;
    float GetFadeValue() const;
    ImColor GetColor() const;

private:
    Type type = Type::NONE;
    std::string title{ "" };
    std::string content{ "" };
    float fadeInTime{ 0.150f };
    float onTime{ 3.0f };
    float fadeOutTime{ 0.150f };
    float opacity{ 1.0f };
    float notifCreateTime{ 0.0f };
};

class Notifier
{
public:
    void Add(const Notif& notif) { AddNotif(notif); }
    void Draw() { DrawNotifications(); }
    void DrawNotifications();

private:
    void AddNotif(const Notif& notif);
    float xPadding{ 30.0f };
    float yPadding{ 30.0f };
    float yMessagePadding{ 10.0f };
    float wrapRatio{ 0.25f };
    float rounding = 8.0f;
    ImVec4 backgroundColor = ImVec4(0.886f, 0.929f, 0.969f, 0.4f);
    std::vector<Notif> notifs;
    float heightNotifs{ 0.0f };
    void RemoveNotif(int i) { notifs.erase(notifs.begin() + i); }
};

#define NOTIF_INFO(title, content) notifier.Add(Notif(Notif::Type::INFO, title, content))
#define NOTIF_WARNING(title, content) notifier.Add(Notif(Notif::Type::WARNING, title, content))
#define NOTIF_ERROR(title, content) notifier.Add(Notif(Notif::Type::ERROR, title, content))
#define NOTIF_SUCCESS(title, content) notifier.Add(Notif(Notif::Type::SUCCESS, title, content))

#endif /* NOTIFIER_HPP */