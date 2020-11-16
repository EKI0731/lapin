#pragma once
class Effects
{
public:
	/// <summary>
	/// �C�[�W���O�֐�
	/// </summary>
	/// <param name="end_position">	�ړ�����</param>
	/// <param name="position">		�ŏ��̈ʒu</param>
	/// <param name="time">			�ړ��t���[��</param>
	/// <param name="now_time">		���݃t���[��</param>
	/// <returns></returns>
	double Ease_In(double end_position, double position, double time, double now_time);
	double Ease_Out(double end_position, double position, double time, double now_time);
	double Ease_InOut(double end_position, double position, double time, double now_time);
};

