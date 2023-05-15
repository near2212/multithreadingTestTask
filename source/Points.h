#pragma once
#include <vector>
#include <atomic>


namespace test
{
	struct PointStruct
	{
		std::atomic<float> _ax;
		std::atomic<float> _ay;
		bool _flag;

		PointStruct()
			:_ax(0), _ay(0), _flag(false)
		{}

		PointStruct(const std::atomic<float>& ax, const std::atomic<float>& ay, const bool flag)

		{
			_ax = ax.load(std::memory_order_relaxed);
			_ay = ay.load(std::memory_order_relaxed);
			_flag = flag;
		}

		PointStruct(const PointStruct& other)

		{
			_ax = other._ax.load(std::memory_order_relaxed);
			_ay = other._ay.load(std::memory_order_relaxed);
			_flag = other._flag;

		}

		PointStruct& operator=(const PointStruct& other)
		{

			_ax.store(other._ax.load(std::memory_order_relaxed), std::memory_order_relaxed);
			_ay.store(other._ay.load(std::memory_order_relaxed), std::memory_order_relaxed);
			_flag = other._flag;
		}
	};



	struct V0_and_g0
	{
		float V0;
		float g;


		explicit V0_and_g0()
			:V0(0), g(0)
		{}
		V0_and_g0(const float V0, const float g)

		{
			this->V0 = V0;
			this->g = g;


		}

		V0_and_g0(const V0_and_g0& other)

		{
			V0 = other.V0;
			g = other.g;


		}
		V0_and_g0& operator=(const V0_and_g0& other)
		{

			V0 = other.V0;
			g = other.g;

		}
		
	};

	struct InitDataStruct
	{
		float x0;
		float y0;
		long long t0;
		bool flag;

		explicit InitDataStruct()
			:x0(0), y0(0), t0(0), flag(false)
		{}
		InitDataStruct(const float x0, const float y0, const long long t0, const bool flag)

		{
			this->x0 = x0;
			this->y0 = y0;

			this->t0 = t0;
			this->flag = flag;
		}

		InitDataStruct(const InitDataStruct& other)

		{
			x0 = other.x0;
			y0 = other.y0;
			t0 = other.t0;
			flag = other.flag;

		}
		InitDataStruct& operator=(const InitDataStruct& other)
		{

			x0 = other.x0;
			y0 = other.y0;
			t0 = other.t0;
			flag = other.flag;
		}

	};

	struct alpha
	{
		float sin_alpha;
		float cos_alpha;


		explicit alpha()
			:sin_alpha(0), cos_alpha(0)
		{}
		alpha(const float sin_alpha, const float cos_alpha)

		{
			this->sin_alpha = sin_alpha;
			this->cos_alpha = cos_alpha;


		}

		alpha(const alpha& other)

		{
			sin_alpha = other.sin_alpha;
			cos_alpha = other.cos_alpha;


		}
		alpha& operator=(const alpha& other)
		{

			sin_alpha = other.sin_alpha;
			cos_alpha = other.cos_alpha;

		}

	};

	struct velocity
	{
		long float Vx;
		long float Vy;


		explicit velocity()
			:Vx(0), Vy(0)
		{}
		velocity(const long float Vx, const long  float Vy)

		{
			this->Vx = Vx;
			this->Vy = Vy;


		}

		velocity(const velocity& other)

		{
			Vx = other.Vx;
			Vy = other.Vy;


		}
		velocity& operator=(const velocity& other)
		{

			Vx = other.Vx;
			Vy = other.Vy;

		}

	};


	class Points
	{
	private:
		std::vector<test::PointStruct> points;
		std::vector<velocity> velocities;
		std::vector<alpha> alphaC;
		std::vector<test::InitDataStruct> intData;
		std::vector<V0_and_g0> V0_g;
	public:

		inline static int startPointCount = 0;
		int pointsSize = 130880;
		int nodesSize = 2024;
		int nodeSize = 64;

		explicit Points()
			
		{
			
		
		}

		void init();


		int RandomGenerator(int num);
		void alpha();
		void velo(int time);
		void position(int time);
		int GenerateValidPosition(int range);
		void AssignNewPoints(int range);
		void SetInitDataOnPosition(int position, long long time);
		void SetInitData(int x, int y, long long time);
		int GetPointX(int position);
		int GetPointY(int position);
		bool GetPointFlag(int position);
		void SetPointX(int position, int x);
		void SetPointY(int position, int y);
		void SetPointFlag(int position, bool flag);
		int GetCurrentInitDatatime();
	};

}
