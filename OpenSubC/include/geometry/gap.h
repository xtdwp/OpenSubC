#pragma once

namespace opensubc {
	//间隙定义
	class gap {
	private:
		static unsigned num;
	public:
		unsigned id;
		unsigned rodId[2];
		int channelId[2];
		double lk, sk;

		gap() = default;
		gap(unsigned rodId0, unsigned rodId1);
		int getOtherChannelId(unsigned _channelId);//获取gap除了输入的通道id之外连接的另一个通道的id
		static int checkGapExistence(unsigned rodId0, unsigned rodId1);
	};
}

