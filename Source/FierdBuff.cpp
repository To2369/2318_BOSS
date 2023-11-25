#include"FierdBuff.h"
#include"PlayerManager.h"

void FierdBuff::primitive()
{
	int sphere = 0;
	DirectX::XMFLOAT3 pos_{};
	//バフ範囲をDrawSphereを使って表示
	DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();
	for (int i = 0; i <= 5; i++)
	{
		
		for (int j = 0; j <= 5; j++)
		{

			pos_={};
			pos_.x = Pos.x + (left * j);
			pos_.z =  Pos.z + (Top * i);
			
			
			debugRender->DrawSphere(pos_, radius, DirectX::XMFLOAT4{ 0,0,0,1 });
			

		}
	}

	//現在のバフパネルの位置
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			pos_ = {};
			pos_.x = Buff_Pos.x + (left2*j);
			pos_.z = Buff_Pos.z + (Top2*i);
			int mag = 0;
			buff(state, i, j,mag);
			debugRender->DrawSphere(pos_, 1.0f, DirectX::XMFLOAT4{ 0,0+(float)mag*0.2f,0,1 });
		}
	}

}

void FierdBuff::Update(float elapsedTime)
{
	number_ = {};
	
	for (int i = 0; i < 5; i++)
	{

		for (int j = 0; j < 5; j++)
		{
			float i_ = (float)(i);
			float j_ = (float)(j);
			
			DirectX::XMFLOAT3 old_pos{};//現在のi,j番目に位置用
			DirectX::XMFLOAT3 next_pos{};//次のi,j番目の位置用
			old_pos.x = Pos.x + (left * j_);
			old_pos.z = Pos.z + (Top * i_);

			next_pos.x = Pos.x + (left * (j_ + 1));
			next_pos.z = Pos.z + (Top * (i_ + 1));
			
			//ここはプレイヤーの処理だからコピーする必要なし
			if (playerisFierdBuffNow(old_pos, next_pos))
			{
				SetPanelBuff(state, i, j);
				DirectX::XMFLOAT2 N{j_,i_};
				number_.x = N.x;
				number_.y = N.y;
				
				break;
			}


		}
	}
}

bool FierdBuff::playerisFierdBuffNow(DirectX::XMFLOAT3 old_pos, DirectX::XMFLOAT3 next_pos)
{
	player* pl = PlayerManager::Instance().GetPlayer(0);
	
	if (pl->GetPosition().x < old_pos.x)return false;
	if (pl->GetPosition().z > old_pos.z)return false;
	if (pl->GetPosition().x > next_pos.x)return false;
	if (pl->GetPosition().z < next_pos.z)return false;

	return true;;
}

void FierdBuff::SetPanelBuff(magnificationPanelState panel,int i,int j)
{
	player* pl = PlayerManager::Instance().GetPlayer(0);
	int mag = 0;
	switch (panel)
	{
	case magnificationPanelState::State1:
		
		buff(panel, i, j, mag);
	pl->Setmagnification(mag);
		break;
	case magnificationPanelState::State2:
	
		buff(panel, i, j, mag);
	pl->Setmagnification(mag);
		break;
	
	}
}

void FierdBuff::buff(magnificationPanelState panel,int i, int j,int& mag)
{
	switch (panel)
	{
	case magnificationPanelState::State1:
		if (j == 1 && i == 0)mag = 2;
		else if (j == 3 && i == 0)mag = 2; 
		else if (j == 0 && i == 2)mag = 4; 
		else if (j == 2 && i == 2)mag = 5;
		else if (j == 4 && i == 2)mag = 4;
		else if (j == 0 && i == 4)mag = 3;
		else if (j == 4 && i == 4)mag = 3;
		else
		{
			mag = 0;
		}

		break;
	case magnificationPanelState::State2:
		if (j == 0 && i == 0)mag = 2;
		else if (j == 2 && i == 0)mag = 3;
		else if (j == 4 && i == 0)mag = 2;
		else if (j == 2 && i == 2)mag = 4;
		else if (j == 0 && i == 4)mag = 2;
		else if (j == 2 && i == 4)mag = 5;
		else if (j == 4 && i == 4)mag = 2;
		else
		{
			mag = 0;
		}

		break;
	}
	
}

std::string FierdBuff::GetStateName()
{
	switch (state)
	{
	case magnificationPanelState::State1:
		return"State1";
		break;
	case magnificationPanelState::State2:
		return "State2";
		break;
	
	}
}
