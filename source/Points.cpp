#pragma once
#include <vector>
#include <atomic>
#include "Points.h"
#include "test.h"
#include <random>
#include <queue>
#include <unordered_set>
#include <cstdint>
#include <future>

int test::Points::RandomGenerator(int num)
{

	std::random_device rd;  // a seed source for the random number engine
	std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(1, num);
	return distrib(gen);
}
//the corner value 
void test::Points::alpha()
{
	for (int i = 0; i < pointsSize; ++i)
	{

		alphaC[i].sin_alpha = RandomGenerator(100) * 0.001;
		if (i % 2)
		{
			alphaC[i].cos_alpha = pow(1 - (alphaC[i].sin_alpha * alphaC[i].sin_alpha), 0.5);
		}
		else
		{
			alphaC[i].cos_alpha = -pow(1 - (alphaC[i].sin_alpha * alphaC[i].sin_alpha), 0.5);
		}
	}
}
//recalculate velocity
void test::Points::velo(int time)
{
	float delta;
	for (int j = 0; j < nodesSize; ++j)
	{
		if (intData[j].flag)
		{
			//convert to seconds
			delta = (time - intData[j].t0) * 0.001;


			for (int i = j * nodeSize; i < j * nodeSize + nodeSize; ++i)
			{
				if (points[i]._flag)
				{
					delta = (time - intData[j].t0) * 0.001;
					velocities[i].Vx = V0_g[i].V0 * alphaC[i].cos_alpha;
					//I do not use Vy in my countings, Vy is calculated on place
					velocities[i].Vy = V0_g[i].V0 * alphaC[i].sin_alpha - V0_g[i].g * delta;
				}
			}
		}

	}

}
//recalculate position of points
void test::Points::position(int time)
{
	float delta;
	for (int j = 0; j < nodesSize; ++j)
	{
		if (intData[j].flag)
		{
			//convert to seconds
			delta = (time - intData[j].t0) * 0.001;


			for (int i = j*nodeSize; i < j * nodeSize +nodeSize; ++i)
			{
				if (points[i]._flag)
				{
					
					points[i]._ax = intData[j].x0 + velocities[i].Vx * delta;

					points[i]._ay = intData[j].y0 + (V0_g[i].V0 * 0.7 * delta - (V0_g[i].g * delta * delta) / 2);

					if (points[i]._ax > test::SCREEN_WIDTH || points[i]._ay > test::SCREEN_HEIGHT || points[i]._ax < 0 || points[i]._ay < 0)
						points[i]._flag = false;
				}
			}
		}
	}

}
//generate position that points to a valid point that is alife and within the points limits
int test::Points::GenerateValidPosition(int range)
{

	int num = 0;
	int count = 0;
	do
	{
		num = RandomGenerator(range);
		count++;
		//1) alife point 2) not to check more than nodeSize,otherwise it runs forever 3) check that we do not return value that is bigger Or equal than our points limits
	} while (!points[num]._flag && count < nodeSize && (num + (startPointCount - 1) * (nodeSize)-1) < pointsSize);

    return num;

}
void test::Points::AssignNewPoints(int range)
{
	//if we are running out of limits we do not want to fall or create new points fill the rest uninitialaized points with new values
	if (range < pointsSize)
	{
		int rightTop = range + nodeSize;
		if ((range + nodeSize) > pointsSize)
		{
			rightTop = pointsSize;
		}

		for (int i = range; i < rightTop; ++i)
		{
			points[i]._ax = intData[startPointCount].x0;
			points[i]._ay = intData[startPointCount].y0;
			points[i]._flag = true;			
		}
	}
	else printf("AssignNewPoints:we are out of point limits\n");

}

void test::Points::init()
{
	 points = std::vector<test::PointStruct>(pointsSize, { 0,0,false });
	velocities = std::vector<test::velocity>(pointsSize, { 0.0,0.0 });
	alphaC = std::vector<test::alpha>(pointsSize, { 0.0,0.0 });
	intData = std::vector<test::InitDataStruct>(nodesSize, { 0,0,0, false });
	 V0_g = std::vector<test::V0_and_g0>(pointsSize, { 0.0,0.0 });
	for (int i = 0; i < V0_g.size(); ++i)
	{
		V0_g[i].V0 = RandomGenerator(6) * RandomGenerator(7);
		V0_g[i].g = RandomGenerator(100);
	}

	alpha();

}

void test::Points::SetInitDataOnPosition(int position, long long time)
{
	//printf("SetInitDataOnPosition  run startPintCount :%d \n", Points::startPointCount);
	if (test::Points::startPointCount < nodesSize)
	{

		intData[test::Points::startPointCount].x0 = static_cast<int>(points[position]._ax);
		intData[test::Points::startPointCount].y0 = static_cast<int>(points[position]._ay);
		intData[test::Points::startPointCount].t0 = time;
		intData[test::Points::startPointCount].flag = true;
	}
	else printf("SetInitDataOnPosition: we are out of point limits\n");
}

void test::Points::SetInitData(int x, int y, long long time)
{
	if (test::Points::startPointCount < nodesSize)	
	{

		intData[Points::startPointCount].x0 = x;
		intData[Points::startPointCount].y0 = test::SCREEN_HEIGHT - y;
		intData[Points::startPointCount].t0 = time;
		intData[Points::startPointCount].flag = true;
	}
	else printf("SetInitData: we are out of point limits\n");
}

int test::Points::GetCurrentInitDatatime()
{
	return intData[test::Points::startPointCount-1].t0;
}

int test::Points::GetPointX(int position)
{
	return points[position]._ax;
}
int test::Points::GetPointY(int position)
{
	return points[position]._ay;
}
bool test::Points::GetPointFlag(int position)
{
	return points[position]._flag;
}

void test::Points::SetPointX(int position,int x)
{
	points[position]._ax = x;
}
void test::Points::SetPointY(int position, int y)
{
	points[position]._ay = y;
}
void test::Points::SetPointFlag(int position, bool flag)
{
	points[position]._flag = flag;
}
