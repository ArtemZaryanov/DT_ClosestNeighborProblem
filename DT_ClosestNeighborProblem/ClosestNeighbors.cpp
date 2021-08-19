#include "ClosestNeighbors.h"

std::map<int, std::vector<int>> ClosestNeighbors::FindNeighborsBrute(const std::vector<UnitInfo>& dataUnit)
{
	auto sqDistance = [](float x, float y) { return x*x + y*y; };
	std::map<int, std::vector<int>> neighbors;
	UnitInfo uM;
	UnitInfo uIt;

	//����� � ��� ���� ���������
	// ����� �������
	//O(n^2)->O(n^2/2)
	for (int i = 0; i < dataUnit.size(); i++)
	{
		uM = dataUnit[i];
		for (int j = 0;  j< dataUnit.size(); j++)
		{

			uIt = dataUnit[j];
			if (sqDistance(uM.posX - uIt.posX, uM.posY - uIt.posY) < uM.SqDistanceView&&i!=j)
			{
				//����� ����������
				neighbors[i].push_back(j);
			}
		}
	}
	int N = 13;
	// ����� �� ������� ���������
	//O(3nN))
	std::vector<Edge> polygon;

	for (int i = 0; i < neighbors.size(); i++)
	{
		//O(2*N));
		polygon = CreatePolygon(dataUnit[i], 13);
		//O(2p[i]N));
		neighbors[i] = FindClosestPointInPolygon(polygon, dataUnit, neighbors[i]);
	}
	return neighbors;
}
std::vector<Edge> ClosestNeighbors::CreatePolygon(const UnitInfo& unitInfo, const int countEdge)
{
	std::vector<Edge> polygon;
	SDL_FPoint* points = new SDL_FPoint[countEdge + 2];
	SDL_FPoint r0{ unitInfo.posX, unitInfo.posY };
	SDL_FPoint d{ unitInfo.directX, unitInfo.directY };
	float dov = sqrtf(unitInfo.SqDistanceView);
	float sectorAngle = unitInfo.sectorAngle * D2R;
	float s = 0;
	float c = 0;
	points[0] = r0;
	//!
	for (int i = -countEdge / 2; i < 0; i++)
	{
		s = sinf(sectorAngle * i / countEdge);
		c = cosf(sectorAngle * i / countEdge);
		points[i + countEdge / 2 + 1].x = (r0.x + dov * (d.x * c - d.y * s));
		points[i + countEdge / 2 + 1].y = (r0.y + dov * (d.y * c + d.x * s));
	}
	if (countEdge % 2 != 0)
	{

		points[countEdge / 2 + countEdge % 2].x = (r0.x + dov * d.x);
		points[countEdge / 2 + countEdge % 2].y = (r0.y + dov * d.y);
	}
	for (int i = 1; i <= countEdge / 2; i++)
	{
		s =  sinf(sectorAngle * i / countEdge);
		c = cosf(sectorAngle * i / countEdge);
		points[i + countEdge / 2 + countEdge % 2].x = (r0.x + dov * (d.x * c - d.y * s));
		points[i + countEdge / 2 + countEdge % 2].y = (r0.y + dov * (d.y * c + d.x * s));
	}
	points[countEdge + 1] = r0;
	for (int i = 1; i <= countEdge + 1; i++)
	{
		polygon.push_back(
			{
				points[i - 1].x,
				points[i - 1].y,
				points[i].x,
				points[i].y,

			}
		);
	}
	delete[] points;
	points = nullptr;
	return polygon;
}

std::vector<int> ClosestNeighbors::FindClosestPointInPolygon(const std::vector<Edge>& polygon, const std::vector<UnitInfo>& dataUnit, const std::vector<int>& neigbors)
{
	std::vector<int> closesPoint;
	UnitInfo unit;
	int j = neigbors.size() - 1;
	bool inPolygon=false;
	float slope = 0;
	for (size_t k = 0; k < neigbors.size(); k++)
	{
		unit = dataUnit[neigbors[k]];
		for (size_t i = 0; i < polygon.size(); i++)
		{
			//in corner
			if ((unit.posX == polygon[i].x0) && (unit.posY == polygon[i].y0))
			{
				inPolygon = true;
				break;
			}
			if ((polygon[i].y0 > unit.posY) != (polygon[j].y0 > unit.posY))
			{
				slope = (unit.posX - polygon[i].x0) * (polygon[j].y0 - polygon[i].y0) - (polygon[j].x0 - polygon[i].x0) * (unit.posY - polygon[i].y0);
				if (slope == 0)
				{
					inPolygon = true;
					break;
				}
				if ((slope < 0) != (polygon[j].y0 < polygon[i].y0))
				{
					inPolygon = !inPolygon;
				}
			}
			j = i;
		}
		if (inPolygon)
		{
			closesPoint.push_back(neigbors[k]);
		}

	}
	return closesPoint;
}
