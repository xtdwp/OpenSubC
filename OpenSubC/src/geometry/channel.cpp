#include "channel.h"
#include <geometry.h>
#include <constants.h>
#include <iostream>
#include <fuelRod.h>
#include <gap.h>

opensubc::channel::channel(unsigned _id, const std::vector<unsigned>& _rodIds, const std::vector<BoundaryType> _boundaryTypes)//����һ������ȼ�ϰ��ͱ߽���Ϣ������ͨ��
    :id(_id), rodIds(_rodIds), boundaryTypes(_boundaryTypes)
{
    originalId = opensubc::geometry::channelIdConverter[id];
    setChannel();
}

void opensubc::channel::setChannel()//�ڳ�ʼ��֮�󣬸���ȼ�ϰ�λ�÷ֲ�������ͨ�����Σ�Ŀǰֻ֧����򵥵ľ������Σ�
{
    std::cout << rodIds.size() << std::endl;
    if (rodIds.size() == 1)
        setCornerChannel();
    else if (rodIds.size() == 2)
        setEdgeChannel();
    else
        setOrdinaryChannel();
}

void opensubc::channel::setCornerChannel()//������ͨ��
{
    using namespace opensubc::geometry;

    fuelRod& rod = rods[rodIds[0]];
    rod.channelIds.push_back(id);//ȼ�ϰ����Ӹ�ͨ��id

    //������ͨ�����ĵ������������
    switch (boundaryTypes[0])
    {
    case BoundaryType::PositiveX:
        x = rod.x + boundaryWidth * 0.5;
        break;
    case BoundaryType::NegativeX:
        x = rod.x - boundaryWidth * 0.5;
        break;
    case BoundaryType::PositiveY:
        y = rod.y + boundaryHeight * 0.5;
        break;
    case BoundaryType::NegativeY:
        y = rod.y - boundaryHeight * 0.5;
        break;
    default:
        break;
    }
    switch (boundaryTypes[1])
    {
    case BoundaryType::PositiveX:
        x = rod.x + boundaryWidth * 0.5;
        break;
    case BoundaryType::NegativeX:
        x = rod.x - boundaryWidth * 0.5;
        break;
    case BoundaryType::PositiveY:
        y = rod.y + boundaryHeight * 0.5;
        break;
    case BoundaryType::NegativeY:
        y = rod.y - boundaryHeight * 0.5;
        break;
    default:
        break;
    }
    A = boundaryHeight * boundaryWidth;

    //����ȼ�ϰ��ڸ�ͨ���ڽ����ܳ�
    circleLength.push_back(rod.r * PI * 0.5);

    //�����߽��ϵ�����gap
    addBoundaryGap();
    addBoundaryGap();

    //������ȼ�ϰ����ӵ�����gap(��Ҫ�жϸü�϶�Ƿ��Ѿ�����)
    addSingleRodGap(rod, boundaryTypes[0]);
    addSingleRodGap(rod, boundaryTypes[1]);
}

void opensubc::channel::setEdgeChannel()//������ͨ��
{
    using namespace opensubc::geometry;

    //��ȼ�ϰ���Ӹ�ͨ��id
    for (auto& rodId : rodIds)
        rods[rodId].channelIds.push_back(id);

    //������ͨ�����ĵ�������������������ܳ�
    if (boundaryTypes[0] == BoundaryType::PositiveX || boundaryTypes[0] == BoundaryType::NegativeX)
    {
        if (boundaryTypes[0] == BoundaryType::PositiveX)
            x = rods[rodIds[0]].x + boundaryWidth * 0.5;
        else
            x = rods[rodIds[0]].x - boundaryWidth * 0.5;
        y = (rods[rodIds[0]].y + rods[rodIds[1]].y) * 0.5;
        A = abs(rods[rodIds[0]].y - rods[rodIds[1]].y) * boundaryWidth;
    }
    else if (boundaryTypes[0] == BoundaryType::PositiveY || boundaryTypes[0] == BoundaryType::NegativeY)
    {
        x = (rods[rodIds[0]].x + rods[rodIds[1]].x) * 0.5;
        if (boundaryTypes[0] == BoundaryType::PositiveY)
            y = rods[rodIds[0]].y + boundaryHeight * 0.5;
        else
            y = rods[rodIds[0]].y - boundaryHeight * 0.5;
        A = abs(rods[rodIds[0]].x - rods[rodIds[1]].x) * boundaryHeight;
    }

    //����ȼ�ϰ��ڸ�ͨ���ڽ����ܳ�
    for (auto& rodId : rodIds)
        circleLength.push_back(rods[rodId].r * PI * 0.5);

    //����һ���߽��ϵ�gap
    addBoundaryGap();

    //����������ȼ�ϰ�gap
    addSingleRodGap(rods[rodIds[0]], boundaryTypes[0]);
    addSingleRodGap(rods[rodIds[1]], boundaryTypes[0]);

    //����һ����ͨgap
    addOrdinaryGap(rods[rodIds[0]], rods[rodIds[1]]);
}

void opensubc::channel::setOrdinaryChannel()//������ͨ��ͨ��
{
    using namespace opensubc::geometry;

    //��ȼ�ϰ���Ӹ�ͨ��id
    for (auto& rodId : rodIds)
        rods[rodId].channelIds.push_back(id);

    //������ͨ�����ĵ�������������������ܳ�
    for (auto& rodId : rodIds)
    {
        x += rods[rodId].x;
        y += rods[rodId].y;
    }
    x /= 4;
    y /= 4;
    A = 4 * abs(x - rods[rodIds[0]].x) * (y - rods[rodIds[0]].y);

    //����ȼ�ϰ��ڸ�ͨ���ڽ����ܳ�
    for (auto& rodId : rodIds)
        circleLength.push_back(rods[rodId].r * PI * 0.5);

    std::cout << "here!" << std::endl;

    //�����ĸ���ͨgap
    fuelRod& rod0 = rods[rodIds[0]];//���봴������ͨ���ĵ�һ��ȼ�ϰ�
    unsigned diagonalRodId;//���һ��ȼ�ϰ��Խǵ�ȼ�ϰ���ţ�����ȼ�ϰ�id��
    //������һ��ȼ�ϰ����������gap
    for (int i = 1; i < 4; ++i)
    {
        fuelRod& rod = rods[rodIds[i]];
        if (abs(rod0.x - rod.x) > rod0.r && abs(rod0.y - rod.y) > rod0.r)//����Խ�,��¼���
            diagonalRodId = i;
        else//�ǶԽ�,����gap
            addOrdinaryGap(rod0, rod);
    }
    //������һ��ȼ�ϰ��Խ�ȼ�ϰ����������gap
    for (int i = 1; i < 4; ++i)
        if (i != diagonalRodId)
            addOrdinaryGap(rods[rodIds[diagonalRodId]], rods[rodIds[i]]);
}

void opensubc::channel::addBoundaryGap()//Ϊ����ͨ�����һ��λ�ڱ߽��gap
{
    using namespace opensubc::geometry;

    //�ڼ�϶����������һ����gap
    gaps.push_back(gap());
    //����id�����ֵ
    setIdOfLastGap();
}

void opensubc::channel::addSingleRodGap(const fuelRod& rod, BoundaryType boundaryType)//Ϊ����ͨ�����һ����ȼ�ϰ���gap
{
    using namespace opensubc::geometry;
    //��Ҫ�ж�gap�Ƿ��Ѿ�������
    unsigned gapId;
    if (checkGapExistence(rod, boundaryType, gapId))//����Ѿ�������
        addExistsGap(gapId);
    else//���δ������
    {
        //����gap����������
        gaps.push_back(gap(rod, boundaryType));
        //����id�����ֵ
        setIdOfLastGap();
    }
}

void opensubc::channel::addOrdinaryGap(const fuelRod& rod0, const fuelRod& rod1)//Ϊ����ͨ�����һ����ͨ������ȼ�ϰ�֮���gap
{
    using namespace opensubc::geometry;
    //��Ҫ�ж�gap�Ƿ��Ѿ�������
    unsigned gapId;
    if (checkGapExistence(rod0, rod1, gapId))//����Ѿ�������
        addExistsGap(gapId);
    else//���δ������
    {
        //����gap����������
        gaps.push_back(gap(rod0, rod1));
        //����id�����ֵ
        setIdOfLastGap();
    }
}

void opensubc::channel::addExistsGap(unsigned gapId)//Ϊ����ͨ�����һ���Ѿ����ڵ�gap
{
    using namespace opensubc::geometry;
    gap& gap = gaps[gapId];

    //����gap�ļ�϶����lk
    channel& channel = channels[gap.channelIds[0]];
    gap.lk = sqrt(pow(channel.x - x, 2) + pow(channel.y - y, 2));

    //��϶���Ӹ���ͨ����id
    gap.channelIds.push_back(id);

    //����ͨ�����Ӽ�϶id
    gapIds.push_back(gapId);
}

void opensubc::channel::setIdOfLastGap()//��gap�����е����һ��gap����id�����Ϣ�����һ��˵���ոմ���,��Ҫ����ʹ�ã�
{
    using namespace opensubc::geometry;
    gap& gap = gaps[gaps.size() - 1];

    //id����Ϊ�����е�λ��
    gap.id = gaps.size() - 1;

    //Ϊ��gap��Ӹ���ͨ����id
    gap.channelIds.push_back(id);

    //Ϊ����ͨ����������ӵ�gap��id
    gapIds.push_back(gap.id);

    //Ϊgap������ȼ�ϰ���Ӹ�gap��id
    for (auto& rodId : gap.rodIds)
        rods[rodId].gapIds.push_back(gap.id);
}

opensubc::channel::~channel()
{

}

std::string opensubc::channel::toString()const
{
    //��ӡ��ͷ
    std::string outputString;
    for (int i = 0; i < 10; ++i)
        outputString += "-";
    outputString += "channel";
    for (int i = 0; i < 10; ++i)
        outputString += "-";
    outputString += "\n";

    //��ӡid��������Ϣ
    outputString += "id: " + std::to_string(id) + ", originalId: " + std::to_string(originalId) + "\n";
    outputString += "x: " + std::to_string(x) + ", y: " + std::to_string(y) + "\n";
    outputString += "A: " + std::to_string(A) + "\n";

    //��ӡ�߽���Ϣ
    outputString += "boundaryTypes: ";
    for (auto& boundaryType : boundaryTypes)
        outputString += std::to_string(static_cast<int>(boundaryType)) + ", ";
    outputString.erase(outputString.size() - 2);
    outputString += "\n";

    //��ӡgap��id
    outputString += "gapIds: ";
    for (auto& gapId : gapIds)
        outputString += std::to_string(gapId) + ", ";
    outputString.erase(outputString.size() - 2);
    outputString += "\n";

    //��ӡȼ�ϰ���id
    outputString += "rodIds: ";
    for (auto& rodId : rodIds)
        outputString += std::to_string(rodId) + ", ";
    outputString.erase(outputString.size() - 2);
    outputString += "\n";

    //��ӡȼ�ϰ������ܳ�
    outputString += "circleLength: ";
    for (auto& length : circleLength)
        outputString += std::to_string(length) + ", ";
    outputString.erase(outputString.size() - 2);
    outputString += "\n";

    //��ӡ��β
    for (int i = 0; i < 27; ++i)
        outputString += "-";
    outputString += "\n";

    return outputString;
}