#pragma once

class NoCopyAble
{
protected:
    NoCopy(const NoCopy &) = delete;
    void operator=(const NoCopy &) = delete;

public:
    NoCopy() = default;
    ~NoCopy() = default;
};
