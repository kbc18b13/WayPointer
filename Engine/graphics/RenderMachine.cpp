#include "stdafx.h"
#include "RenderMachine.h"
#include "IRenderObject.h"

RenderMachine::RenderMachine()
{
}


RenderMachine::~RenderMachine()
{
}

void RenderMachine::Render(){
    for( IRenderObject* ro : roArray ){
        ro->Render();
    }
}

void RenderMachine::AddRenderObject( IRenderObject * ro ){
	ro->machine = this;
    ro->index = roArray.size();
    roArray.push_back( ro );
}

void RenderMachine::RemoveRenderObject( IRenderObject * ro ){
    //指定されたインデックスを末尾で上書き
    roArray.back()->index = ro->index;
    roArray[ro->index] = roArray.back();

    //不要になった末尾を削除
    roArray.pop_back();
}
