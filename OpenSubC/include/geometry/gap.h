#pragma once

namespace opensubc {
	//��϶����
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
		int getOtherChannelId(unsigned _channelId);//��ȡgap���������ͨ��id֮�����ӵ���һ��ͨ����id
		static int checkGapExistence(unsigned rodId0, unsigned rodId1);
	};
}

