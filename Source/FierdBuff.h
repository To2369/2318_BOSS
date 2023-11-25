#pragma once
//左上　X=-17.901,Z=12.505
//左下　X=-17.901,Z=-12.505
//右上　X=17.901,Z=12.505
//右下　X=17.901,Z=-12.505
#include"Graphics/Graphics.h"
#include<DirectXMath.h>
#include<vector>
enum class magnificationPanelState
{
	State1,
	State2,
};
class FierdBuff
{
public:
	static FierdBuff& Incetance()
	{
		static FierdBuff inc_;
		return inc_;
	}
	FierdBuff() {};
	~FierdBuff() {};
	void primitive();
	void Update(float elapsedTime);
	bool playerisFierdBuffNow(DirectX::XMFLOAT3 old_pos, DirectX::XMFLOAT3 next_pos);//今playerが位置がバフ効果範囲内かどうか
public://Set
	void SetNumber(DirectX::XMFLOAT2 num) { number_ = num; }
	
	void SetPanelBuff(magnificationPanelState panel, int i, int j);
	void SetBuffPanelState(magnificationPanelState st) { 
		state = st;
	}

public://Get
	void buff(magnificationPanelState panel,int i,int j,int&mag);
	DirectX::XMFLOAT2 GetNumber() {return number_;}
	magnificationPanelState GetBuffPanelState() { return state; }
	std::string GetStateName();
private:
	magnificationPanelState state= magnificationPanelState::State1;
	DirectX::XMFLOAT3 Pos{ -17.901f,-0.100,12.505f};
	DirectX::XMFLOAT3 Buff_Pos{ -14.321f,-0.100,9.995f};
	float radius=0.5f;
	float left = 7.16f;
	float Top = -5.02f;
	float left2 = 7.16f;
	float Top2 = -5.02f;
	DirectX::XMFLOAT2 number_{};


};
