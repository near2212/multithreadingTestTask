#pragma once
#include <thread>
#include <mutex>
#include <atomic>


#include "./nvToolsExt.h"

#include "test.h"
#include <random>
#include <queue>
#include <unordered_set>
#include <cstdint>
#include <future>


// Just some hints on implementation
// You could remove all of them

//TODO create a tree over vector node to store the points
//get reed of the logic based on startPointCount as it makes impossible to make a lot of clicks very quick and receive a correct result


static std::atomic_int globalTime;
static std::atomic_int startTime;
static std::atomic_bool workerMustExit = false;

static std::atomic_bool counterMustExit = false;
static std::atomic_bool blowDestroyMustExit = false;
static std::thread workerThread;
static std::thread counterThread;
static std::thread blowDestroyThread;

test::Points p;

bool hitOnce = false;
std::chrono::steady_clock::time_point begin;

static std::atomic_bool flag = false;




void WorkerThread(void)
{
	while (!workerMustExit.load(std::memory_order_relaxed))
	{
		nvtxRangePush(__FUNCTION__);

		static int lastTime = 0;
		const int time = globalTime.load(std::memory_order_relaxed);
		const int delta = time - lastTime;
		lastTime = time;

		//if (delta > 0)
		//{
			test::render();
		//}

		static const int MIN_UPDATE_PERIOD_MS = 10;
		if (delta < MIN_UPDATE_PERIOD_MS)
			std::this_thread::sleep_for(std::chrono::milliseconds(MIN_UPDATE_PERIOD_MS - delta));

		nvtxRangePop();
	}
}

//initialase here zero velocity and G value, I do not use here 9.8 m/s*s as it is to fast
void test::init(void)
{
	printf("init\n");
	p.init();
	// some code

	workerThread = std::thread(WorkerThread);
	begin = std::chrono::steady_clock::now();

	// some code
}

void test::term(void)
{
	printf("term\n");
	// some code

	workerMustExit = true;
	counterMustExit = true;
	blowDestroyMustExit = true;


	// some code

	workerThread.join();
	counterThread.join();

	// some code
}
//pass here ready matrix
void test::render(void)
{
	// some code

		for (int i = 0; i < p.pointsSize; ++i)
		{
			if (p.GetPointFlag(i))
			{
				platform::drawPoint(p.GetPointX(i), p.GetPointY(i), 1, 1, 1, 1);

			}
		}
	

	// some code
}
//
void test::update(int dt)
{
	// some code
	globalTime.fetch_add(dt);
	// some code
}

void PointsThread(void)
{
	while (!counterMustExit)
	{		
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

		p.velo(time);
		p.position(time);			
	}
}

void BlowOrDestroyThread()
{
		while (!blowDestroyMustExit)
		{
			if (flag.load())
			{
				flag.store(false);

				std::this_thread::sleep_for(std::chrono::milliseconds(1500));

				int add = (test::Points::startPointCount - 1) * p.nodeSize;
				for (int i = 0; i < p.nodeSize; ++i)
				{
					int range = (test::Points::startPointCount)*p.nodeSize;
					int position = p.GenerateValidPosition(p.nodeSize - 1) + add;
					if (i % 2)
					{
						std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
						int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
						p.SetInitDataOnPosition(position, time);
						p.AssignNewPoints(range);
						test::Points::startPointCount++;
					}
					p.SetPointFlag(position, false);
				}
				
			}
		}
}

// click on window initiate first points node and starts counter thread
void test::on_click(int x, int y)
{
	printf("on_click\n");
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	if (!hitOnce)
	{
		int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		p.SetInitData(x, y, time);
		p.AssignNewPoints(0);

		Points::startPointCount++;

		counterThread = std::thread(PointsThread);
		blowDestroyThread = std::thread(BlowOrDestroyThread);
		blowDestroyThread.detach();

		flag.store(true);
		hitOnce = true;
	}
	else if(Points::startPointCount < p.nodesSize-1)
	{
		int range = Points::startPointCount * p.nodeSize;
		int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		p.SetInitData(x, y, time);

		p.AssignNewPoints(range);
		Points::startPointCount++;

		flag.store(true);
	}


}





