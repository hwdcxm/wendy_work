#ifndef _PROTOCOL_SRV_H_
#define _PROTOCOL_SRV_H_

#pragma pack(1)

#define	START_FRAME			('WS')			//������Э����ʼFrame�ı�־
#define DATAOFFSET_FRAME	('DF')			//����DA��WS��λ��
#define	REQUESTC_FRAME		('UC')			//������������
#define	REQUESTD_FRAME		('UD')			//����Ͽ�����
#define	REQUESTR_FRAME		('RQ')			//������ʷ����
#define	USERID_FRAME		('UI')			//User ID
#define SERCONN_FRAME		('CS')			//����������
#define SERDISC_FRAME		('DS')			//�������Ͽ�
#define DATAPACK_FRAME		('DA')			//�������Ͽ�

#define REQUEST_CT			('CT')			//UC/UD/RQ�������ΪCharting

#pragma pack(8)

#endif