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
    //�w�肳�ꂽ�C���f�b�N�X�𖖔��ŏ㏑��
    roArray.back()->index = ro->index;
    roArray[ro->index] = roArray.back();

    //�s�v�ɂȂ����������폜
    roArray.pop_back();
}
