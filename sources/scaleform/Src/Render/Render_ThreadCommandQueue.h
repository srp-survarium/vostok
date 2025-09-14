/**************************************************************************

PublicHeader:   Render
Filename    :   Render_ThreadCommandQueue.h
Content     :   CommandQueue and command items interfaces for the
                render thread. 
Created     :   January 19, 2011
Authors     :   Michael Antonov

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#ifndef INC_SF_Render_ThreadCommandQueue_H
#define INC_SF_Render_ThreadCommandQueue_H

#include "Kernel/SF_RefCount.h"

namespace Scaleform { namespace Render {


// ThreadCommand is a base class for a command that is expected to be executed
// on a render thread.
class ThreadCommand : public RefCountBase<ThreadCommand, Stat_Default_Mem>
{
public:
    virtual void Execute() = 0;
};

//--------------------------------------------------------------------
// ***** ThreadCommandQueue

// ThreadCommandQueue is a command interface that should be implemented by
// a RenderThread and passed to Render::HAL. ThreadCommand-derived objects
// will be pushed on it for execution by the render thread.
//
// Currently used for servicing TextureManager texture creation and
// render Context shutdown requests, which need to be handled quickly even
// when the render thread has no other work.

class ThreadCommandQueue
{
public:
	virtual ~ThreadCommandQueue() {}

    // Pushes a command to be executed, it is expected that command will be
    // AddRef-ed until executed on the render thread.
    virtual void PushThreadCommand(ThreadCommand* command) = 0;
};


}} // Scaleform::Render

#endif
