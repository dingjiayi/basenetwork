/*
 Copyright 2018, Junyi Liu. All rights reserved.
*/
#pragma once

#include <string>

class noncopyable
{
    protected:
    noncopyable() {}
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
};

struct copyable
{
    
};