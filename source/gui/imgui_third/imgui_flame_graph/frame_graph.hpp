#pragma once
#include <imgui.h>
#include "imgui_widget_flamegraph.h"

#include <array>
#include <chrono>
#include <map>

class Profiler
{
public:

    struct Scope
    {
        ImU8 _level;
        std::chrono::system_clock::time_point _start;
        std::chrono::system_clock::time_point _end;
        bool _finalized = false;
        std::string _name;
    };

    struct Entry
    {
        std::chrono::system_clock::time_point _frameStart;
        std::chrono::system_clock::time_point _frameEnd;
        //std::array<Scope, _StageCount> _stages;
        std::map<int, Scope> _stages;
        std::map<std::string,int> _stageIndex;
    };

    void Frame()
    {
        auto& prevEntry = _entries[_currentEntry];
        _currentEntry = (_currentEntry + 1) % _bufferSize;
        prevEntry._frameEnd = _entries[_currentEntry]._frameStart = std::chrono::system_clock::now();
    }

    void Begin(std::string name)
    {
        assert(_currentLevel < 255);
        if (_entries[_currentEntry]._stageIndex.find(name) == _entries[_currentEntry]._stageIndex.end())
		{
			_entries[_currentEntry]._stageIndex[name] = (int)_entries[_currentEntry]._stages.size();
		}
        auto& entry = _entries[_currentEntry]._stages[_entries[_currentEntry]._stageIndex[name]];
        entry._level = _currentLevel;
        _currentLevel++;
        entry._start = std::chrono::system_clock::now();
        entry._finalized = false;
        entry._name = name;
    }

    void End(std::string name)
    {
        assert(_currentLevel > 0);
        auto& entry = _entries[_currentEntry]._stages[_entries[_currentEntry]._stageIndex[name]];
        assert(!entry._finalized);
        _currentLevel--;
        assert(entry._level == _currentLevel);
        entry._end = std::chrono::system_clock::now();
        entry._finalized = true;
    }

    ImU8 GetCurrentEntryIndex() const
    {
        return (_currentEntry + _bufferSize - 1) % _bufferSize;
    }

    static const ImU8 _bufferSize = 10;
    std::array<Entry, _bufferSize> _entries;

private:
    ImU8 _currentEntry = _bufferSize - 1;
    ImU8 _currentLevel = 0;
};


static void ProfilerValueGetter(float* startTimestamp, float* endTimestamp, ImU8* level, const char** caption, const void* data, int idx)
{
    auto entry = reinterpret_cast<const Profiler::Entry*>(data);
    auto& stage = entry->_stages.at(idx);
    if (startTimestamp)
    {
        std::chrono::duration<float, std::milli> fltStart = stage._start - entry->_frameStart;
        *startTimestamp = fltStart.count();
    }
    if (endTimestamp)
    {
        *endTimestamp = stage._end.time_since_epoch().count() / 1000000.0f;

        std::chrono::duration<float, std::milli> fltEnd = stage._end - entry->_frameStart;
        *endTimestamp = fltEnd.count();
    }
    if (level)
    {
        *level = stage._level;
    }
    if (caption)
    {
        *caption = stage._name.c_str();
    }
}
