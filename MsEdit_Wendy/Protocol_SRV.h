#ifndef _PROTOCOL_SRV_H_
#define _PROTOCOL_SRV_H_

#pragma pack(1)

#define	START_FRAME			('WS')			//服务器协议起始Frame的标志
#define DATAOFFSET_FRAME	('DF')			//数据DA距WS的位移
#define	REQUESTC_FRAME		('UC')			//请求数据连接
#define	REQUESTD_FRAME		('UD')			//请求断开数据
#define	REQUESTR_FRAME		('RQ')			//请求历史数据
#define	USERID_FRAME		('UI')			//User ID
#define SERCONN_FRAME		('CS')			//服务器连接
#define SERDISC_FRAME		('DS')			//服务器断开
#define DATAPACK_FRAME		('DA')			//服务器断开

#define REQUEST_CT			('CT')			//UC/UD/RQ请求类别为Charting

#pragma pack(8)

#endif