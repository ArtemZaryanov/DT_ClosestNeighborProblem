#include "ClosestNeighbors.h"

ClosestNeighbors::ClosestNeighbors(const Settings& settings, const int countEdge)
{
	sqDOV = settings.SqDistanceView;
	sectorAngle = settings.sectorAngle;
	originPolygon = CreateOriginPolygon(countEdge);
}

std::map<int, std::vector<int>> ClosestNeighbors::FindNeighborsBrute(const std::vector<UnitInfo>& dataUnit)
{
	auto sqDistance = [](int x, int y) { return x*x + y*y; };
	std::map<int, std::vector<int>> neighbors;
	UnitInfo uM;
	UnitInfo uIt;

	//Можно в два раза сократить
	// Поиск соседей
	//O(n^2)->O(n^2/2)
	for (int i = 0; i < dataUnit.size(); i++)
	{
		uM = dataUnit[i];
		for (int j = 0;  j< dataUnit.size(); j++)
		{

			uIt = dataUnit[j];
			if (sqDistance(uM.posX - uIt.posX, uM.posY - uIt.posY) < sqDOV&&i!=j)
			{
				//Будут повторения
				neighbors[i].push_back(j);
			}
		}
	}
	int N = 13;
	// Отбор по области видимости
	//O(3nN))
	std::vector<Edge> rotatingPolygon;
	float angle;
	for (int i = 0; i < neighbors.size(); i++)
	{
		//O(2*N));
		//RotatePolygon
		rotatingPolygon = RotatePolygon(dataUnit[i].directX, dataUnit[i].directY,dataUnit[i]);
		//O(2p[i]N));
		neighbors[i] = FindClosestPointInPolygon(rotatingPolygon, dataUnit, neighbors[i]);
	}
	return neighbors;
}
std::vector<Edge> ClosestNeighbors::CreateOriginPolygon(const int countEdge)
{
	std::vector<Edge> polygon;
	SDL_FPoint* points = new SDL_FPoint[countEdge + 2];
	SDL_FPoint r0{ 0,0 };
	SDL_FPoint d{ 1, 0};
	float dov = sqrtf((float)sqDOV);
	float sectorAngleRadians = sectorAngle * D2R;
	float s = 0;
	float c = 0;
	points[0] = r0;
	//!
	for (int i = -countEdge / 2; i < 0; i++)
	{
		s = sinf(sectorAngleRadians * i / countEdge);
		c = cosf(sectorAngleRadians * i / countEdge);
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
		s =  sinf(sectorAngleRadians * i / countEdge);
		c = cosf(sectorAngleRadians * i / countEdge);
		points[i + countEdge / 2 + countEdge % 2].x = (r0.x + dov * (d.x * c - d.y * s));
		points[i + countEdge / 2 + countEdge % 2].y = (r0.y + dov * (d.y * c + d.x * s));
	}
	points[countEdge + 1] = r0;
	for (int i = 0; i <= countEdge + 1; i++)
	{
		polygon.push_back(
			{
				points[i].x,
				points[i].y,
				//points[i].x,
				//points[i].y,

			}
		);
	}
	delete[] points;
	points = nullptr;
	return polygon;
}

std::vector<Edge> ClosestNeighbors::RotatePolygon(const float sin, const float cos,const UnitInfo& unitInfo)
{
	std::vector<Edge> rotatingPolygon;
	for (const auto& edje : originPolygon)
	{
		rotatingPolygon.push_back({
			unitInfo.posX + edje.x0 * cos - edje.y0 * sin,
			unitInfo.posY + edje.x0 * sin + edje.y0 * cos
			});
	}
	return rotatingPolygon;
}

std::vector<int> ClosestNeighbors::FindClosestPointInPolygon(const std::vector<Edge>& polygon, const std::vector<UnitInfo>& dataUnit, const std::vector<int>& neigbors)
{
	std::vector<int> closesPoint;
	UnitInfo unit;
	size_t j = polygon.size() - 1;
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
