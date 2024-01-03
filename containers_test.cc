

#include <array>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>

#include <algorithm>
#include <chrono>
#include <thread>



//#include <boost/container/devector.hpp>
#include <boost/container/container_fwd.hpp>
#include <boost/container/stable_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <boost/container/small_vector.hpp>
#include <boost/container/flat_set.hpp>
#include "boost/test/unit_test.hpp"
//BOOST_AUTO_TEST_SUITE(TstContainers)

    using namespace std::chrono_literals;
    class SlowCopy
    {
     public :

        SlowCopy(int init) noexcept
            : dummy_(init)
        {
        }
        SlowCopy() noexcept
            : dummy_(0)
        {
        }
        SlowCopy(const SlowCopy& iother) noexcept
        {
          dummy_ = iother.dummy_;
          std::this_thread::sleep_for(5ms);
        }
        SlowCopy& operator=(const SlowCopy& iother) noexcept
        {
          dummy_ = iother.dummy_;
          std::this_thread::sleep_for(5ms);
          return *this;
        }
        // Move blocked
        SlowCopy& operator=(SlowCopy&& iother) noexcept
        {
          dummy_ = iother.dummy_;
          std::this_thread::sleep_for(5ms);
          return *this;
        }
        SlowCopy(SlowCopy&& iother) noexcept
        {
          dummy_ = iother.dummy_;
          std::this_thread::sleep_for(5ms);
        }
     private:
        int dummy_;
    };

    BOOST_AUTO_TEST_CASE(array)
    {

      { //No reserve int type
        constexpr int NBPUSH = 100000;
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        std::array<int, NBPUSH> Array;
        BOOST_TEST_MESSAGE("[array] sizeof " << sizeof(Array) << " BYTES");
        size_t Pos = 0;
        for (int i = 0; i < NBPUSH; i++)
          Array[i] = i;

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[array] Init " << time_span.count() << "s");
      }


      // { //No reserve int type
      // const int NBPUSH=1'000'000'000;
      // BOOST_TEST_MESSAGE("---Problem Size =" <<NBPUSH);
      //    auto Start = std::chrono::high_resolution_clock::now();
      //    std::array<int,NBPUSH> Array;
      //    size_t Pos=0;
      //    for(int i=0;i<NBPUSH;i++)
      //    Array[i]=i;

      //    std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
      //    BOOST_TEST_MESSAGE("[vector] PushBack No Reserve " << time_span.count() << "s");
      // }



    }

    BOOST_AUTO_TEST_CASE(array_find)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---FIND 10 MULT Problem Size =" << NBPUSH);

        std::array<int, NBPUSH> Array;
        size_t Pos = 0;
        for (int i = 0; i < NBPUSH; i++)
          Array[i] = i;
        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        for(int j=0;j<NBPUSH;j+=10)
        {
          NbEven+=std::find(Array.begin(), Array.end(),j)!=Array.end() ? 1 :0;
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[array] FIND 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[array] NbEven " << NbEven);
      }

      { // find  type
        const int NBFIND = 1'000'000;
        const int SIZEVEC = 4;
        BOOST_TEST_MESSAGE("---FIND EVEN Nb FIND LOOP =" << NBFIND<<" in Container of "<<SIZEVEC<<" elements" );

        std::array<int, SIZEVEC> Array={4,3,2,1};

        auto Start = std::chrono::high_resolution_clock::now();

        size_t NbEven=0;
        for (int i = NBFIND; i > 0; i--)
          for(int j=0;j<SIZEVEC;j+=2)
          {
            NbEven+=std::find(Array.begin(), Array.end(),j)!=Array.end() ? 1 :0;
          }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[array] Find Even in Small  " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[array] NbEven " << NbEven);
      }


    }

    BOOST_AUTO_TEST_CASE(vector)
    {

      { //No reserve int type
        const int NBPUSH = 100'000'000;
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        std::vector<int> NoReserve;
        BOOST_TEST_MESSAGE("[vector] sizeof " << sizeof(NoReserve) << " BYTES");
        for (int i = NBPUSH; i > 0; i--)
          NoReserve.push_back(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[vector] PushBack No Reserve " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[vector] PushBack No Reserve Size : " << NoReserve.size());
        BOOST_TEST_MESSAGE("[vector] PushBack No Reserve Capacity : " << NoReserve.capacity());
        BOOST_TEST_MESSAGE(
            "[vector] Waisted Mem : " << 100. * (NoReserve.capacity() - NoReserve.size()) / static_cast<double>(NoReserve.size()) << "%");
      }

      { // reserve int type
        const int NBPUSH = 100'000'000;
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        std::vector<int> Reserve;
        //
        Reserve.reserve(NBPUSH);
        //
        for (int i = NBPUSH; i > 0; i--)
          Reserve.push_back(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[vector] PushBack With Reserve " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[vector] PushBack With Reserve Size " << Reserve.size());
        BOOST_TEST_MESSAGE("[vector] PushBack With Reserve Capacity " << Reserve.capacity());
        BOOST_TEST_MESSAGE("[vector] Waisted Mem : " << 100. * (Reserve.capacity() - Reserve.size()) / static_cast<double>(Reserve.size()) << "%");
      }
      { // under reserve int type
        const int NBPUSH = 100'000'000;
        BOOST_TEST_MESSAGE("---(RESERVE)Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        std::vector<int> Reserve;
        //
        Reserve.reserve((2 * NBPUSH) / 3);
        //
        for (int i = NBPUSH; i > 0; i--)
          Reserve.push_back(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[vector] PushBack With  Reserve2/3 " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[vector] PushBack With Reserve2/3 Size " << Reserve.size());
        BOOST_TEST_MESSAGE("[vector] PushBack With Reserve2/3 Capacity " << Reserve.capacity());
        BOOST_TEST_MESSAGE("[vector] Waisted Mem : " << 100. * (Reserve.capacity() - Reserve.size()) / static_cast<double>(Reserve.size()) << "%");
      }







      { // under reserve int type


        const size_t ALLOC = 8'000'000'000;
        BOOST_TEST_MESSAGE("--- BIG ALLOC@once " << ALLOC);
        auto Start = std::chrono::high_resolution_clock::now();
        std::vector<int> Reserve(ALLOC, 0);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[vector]BIG ALLOC@once " << time_span.count() << "s");
      }

      { // under reserve int type
        const size_t ALLOC = 8'000'000'000;

        auto Start = std::chrono::high_resolution_clock::now();
        constexpr size_t NbChunk = 200;
        constexpr size_t ATOMIC_SIZE = ALLOC / NbChunk;
        BOOST_TEST_MESSAGE("--- " << NbChunk << " vec smaller ALLOC (by push_back+copy) of " << ATOMIC_SIZE);
        //std::array<std::vector<int>, NbChunk> SmallAlloc={{ATOMIC_SIZE,0}};
        std::vector<std::vector<int>> VecofBigVec;
        for (int i = 0; i < NbChunk; ++i)
        {
          std::vector<int> ABigVec(ATOMIC_SIZE, 0);
          VecofBigVec.push_back(ABigVec);//will be moved
        }
        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[vector]BIG ALLOC@once " << time_span.count() << "s");
      }

      { // under reserve int type
        const size_t ALLOC = 8'000'000'000;

        auto Start = std::chrono::high_resolution_clock::now();
        constexpr size_t NbChunk = 200;
        constexpr size_t ATOMIC_SIZE = ALLOC / NbChunk;
        BOOST_TEST_MESSAGE("--- " << NbChunk << " vec smaller ALLOC (by push_back+move) of " << ATOMIC_SIZE);
        //std::array<std::vector<int>, NbChunk> SmallAlloc={{ATOMIC_SIZE,0}};
        std::vector<std::vector<int>> VecofBigVec;
        for (int i = 0; i < NbChunk; ++i)
        {
          std::vector<int> ABigVec(ATOMIC_SIZE, 0);
          VecofBigVec.push_back(std::move(ABigVec));//will be moved
        }
        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[vector]BIG ALLOC@once " << time_span.count() << "s");
      }

      { // under reserve int type
        const size_t ALLOC = 8'000'000'000;

        auto Start = std::chrono::high_resolution_clock::now();
        constexpr size_t NbChunk = 200;
        constexpr size_t ATOMIC_SIZE = ALLOC / NbChunk;
        BOOST_TEST_MESSAGE("--- " << NbChunk << " vec smaller ALLOC (by emplace_back) of " << ATOMIC_SIZE);
        //std::array<std::vector<int>, NbChunk> SmallAlloc={{ATOMIC_SIZE,0}};
        std::vector<std::vector<int>> VecofBigVec;
        for (int i = 0; i < NbChunk; ++i)
        {
          VecofBigVec.emplace_back(ATOMIC_SIZE, 0);//will be moved
        }
        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[vector]BIG ALLOC@once " << time_span.count() << "s");
      }

    }


    BOOST_AUTO_TEST_CASE(vec_find)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---FIND 10 MULT Problem Size =" << NBPUSH);

        std::vector<int> vec;
        //
        vec.reserve(NBPUSH);
        //
        for (int i = NBPUSH; i > 0; i--)
          vec.push_back(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        for(int j=0;j<NBPUSH;j+=10)
        {
          NbEven+=std::find(vec.begin(), vec.end(),j)!=vec.end() ? 1 :0;
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[vector] FIND 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[vector] NbEven " << NbEven);
      }

      { // find  type
        const int NBFIND = 1'000'000;
        const int SIZEVEC = 4;
        BOOST_TEST_MESSAGE("---FIND EVEN Nb FIND LOOP =" << NBFIND<<" in Container of "<<SIZEVEC<<" elements" );

        std::vector<int> vec;
        //
        vec.reserve(SIZEVEC);
        //
        for (int i = SIZEVEC; i > 0; i--)
          vec.push_back(i);
        auto Start = std::chrono::high_resolution_clock::now();

        size_t NbEven=0;
        for (int i = NBFIND; i > 0; i--)
          for(int j=0;j<SIZEVEC;j+=2)
          {
            NbEven+=std::find(vec.begin(), vec.end(),j)!=vec.end() ? 1 :0;
          }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[vector] Find Even in Small VEC " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[vector] NbEven " << NbEven);
      }


    }

    BOOST_AUTO_TEST_CASE(vec_emplace)
    {
      { //No reserve SlowCopy type
        const int NBPUSH = 10'000;
        BOOST_TEST_MESSAGE("---SlowCopy Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        std::vector<SlowCopy> NoReserve;
        BOOST_TEST_MESSAGE("[vector] sizeof " << sizeof(NoReserve) << " BYTES");
        for (int i = NBPUSH; i > 0; i--)
        {
          SlowCopy Tmp(i);
          NoReserve.push_back(Tmp);
        }
        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[vector] SlowCopy PushBack No Reserve " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[vector] SlowCopy PushBack No Reserve Size : " << NoReserve.size());
        BOOST_TEST_MESSAGE("[vector] SlowCopy PushBack No Reserve Capacity : " << NoReserve.capacity());
        BOOST_TEST_MESSAGE(
            "[vector] SlowCopy Waisted Mem : " << 100. * (NoReserve.capacity() - NoReserve.size()) / static_cast<double>(NoReserve.size()) << "%");
      }

      { // reserve int type
        const int NBPUSH = 10'000;
        BOOST_TEST_MESSAGE("---SlowCopy Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        std::vector<SlowCopy> Reserve;
//
        Reserve.reserve(NBPUSH);
//
        for (int i = NBPUSH; i > 0; i--)
          Reserve.emplace_back(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[vector] SlowCopy EmplaceBack With Reserve " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[vector] SlowCopy EmplaceBack With Reserve Size " << Reserve.size());
        BOOST_TEST_MESSAGE("[vector] SlowCopy EmplaceBack With Reserve Capacity " << Reserve.capacity());
        BOOST_TEST_MESSAGE(
            "[vector] SlowCopy Waisted Mem : " << 100. * (Reserve.capacity() - Reserve.size()) / static_cast<double>(Reserve.size()) << "%");
      }
    }


    BOOST_AUTO_TEST_CASE(vec_insert)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---INSERT  Problem Size =" << NBPUSH);

        std::vector<int> vec;
        //
        vec.reserve(NBPUSH);
        //
        for (int i = NBPUSH; i > 0; i--)
          vec.push_back(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        
        for(int j=0;j<NBPUSH;j+=10)
        {
           auto insertpos=std::next(vec.begin(), (NBPUSH/2));
          vec.insert(insertpos,-j);
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[vector] INSERT 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[vector] New Size " << vec.size());
        BOOST_TEST_MESSAGE("[vector] New Capacity " << vec.capacity());
        BOOST_TEST_MESSAGE("[vector] Waisted Mem : " << 100. * (vec.capacity() - vec.size()) / static_cast<double>(vec.size()) << "%");

        std::vector<int> vec2(vec);
        BOOST_TEST_MESSAGE("[vector] Vec2 New Size " << vec2.size());
        BOOST_TEST_MESSAGE("[vector] Vec2 New Capacity " << vec2.capacity());
        BOOST_TEST_MESSAGE("[vector] Vec2 Waisted Mem : " << 100. * (vec2.capacity() - vec2.size()) / static_cast<double>(vec2.size()) << "%");


        BOOST_TEST_MESSAGE("---hand erase ----");
        Start = std::chrono::high_resolution_clock::now();
        auto it=vec.begin();
while(it!=vec.end())
{
  if(*it<0)
    it=vec.erase(it);
  else ++it;
}
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[vector] hand erase  " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[vector] New Size " << vec.size());


        BOOST_TEST_MESSAGE("---remove if + erase ---");
        Start = std::chrono::high_resolution_clock::now();
        auto NewEnd=std::remove_if(vec2.begin(),vec2.end(), [](int i){ return i<0;});
        BOOST_TEST_MESSAGE("[vector]  Size after removeif " << vec2.size());
        vec2.erase(NewEnd,vec2.end());
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[vector] remove+erase" << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[vector] New Size " << vec2.size());
      }




    }

    BOOST_AUTO_TEST_CASE(deque)
    {
      const int NBPUSH = 100'000'000;
      { //pushback
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        std::deque<int> deque;
        BOOST_TEST_MESSAGE("[deque] sizeof " << sizeof(deque) << " BYTES");
        for (int i = NBPUSH; i > 0; i--)
          deque.push_back(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[deque] PushBack " << time_span.count() << "s");

      }

      { //pushfront
        auto Start = std::chrono::high_resolution_clock::now();
        std::deque<int> deque;
        for (int i = NBPUSH; i > 0; i--)
          deque.push_front(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[deque] PushFront " << time_span.count() << "s");

      }

    }




    BOOST_AUTO_TEST_CASE(deque_find)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---FIND 10 MULT Problem Size =" << NBPUSH);

        std::deque<int> vec;

        for (int i = NBPUSH; i > 0; i--)
          vec.push_back(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        for(int j=0;j<NBPUSH;j+=10)
        {
          NbEven+=std::find(vec.begin(), vec.end(),j)!=vec.end() ? 1 :0;
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[deque] FIND 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[deque] NbEven " << NbEven);
      }

      { // find  type
        const int NBFIND = 1'000'000;
        const int SIZEVEC = 4;
        BOOST_TEST_MESSAGE("---FIND EVEN Nb FIND LOOP =" << NBFIND<<" in Container of "<<SIZEVEC<<" elements" );

        std::deque<int> vec;

        for (int i = SIZEVEC; i > 0; i--)
          vec.push_back(i);
        auto Start = std::chrono::high_resolution_clock::now();

        size_t NbEven=0;
        for (int i = NBFIND; i > 0; i--)
          for(int j=0;j<SIZEVEC;j+=2)
          {
            NbEven+=std::find(vec.begin(), vec.end(),j)!=vec.end() ? 1 :0;
          }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[deque] Find Even in Small VEC " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[deque] NbEven " << NbEven);
      }


    }

    BOOST_AUTO_TEST_CASE(deque_insert)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---INSERT  Problem Size =" << NBPUSH);

        std::deque<int> vec;
        //

        //
        for (int i = NBPUSH; i > 0; i--)
          vec.push_back(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        //auto insertpos=std::next(vec.begin(), (NBPUSH/2));
        for(int j=0;j<NBPUSH;j+=10)
        {
          //BOOST_TEST_CHECKPOINT("j"+std::to_string(j));
          auto insertpos=std::next(vec.begin(), (NBPUSH/2));
          vec.insert(insertpos,-j);
        }

        BOOST_TEST_CHECKPOINT("FIN");
        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[deque] INSERT 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[deque] New Size " << vec.size());


        std::deque<int> vec2(vec);
        BOOST_TEST_MESSAGE("[deque] Vec2 New Size " << vec2.size());



        BOOST_TEST_MESSAGE("---hand erase ----");
        Start = std::chrono::high_resolution_clock::now();
        auto it=vec.begin();
        while(it!=vec.end())
        {
          if(*it<0)
            it=vec.erase(it);
          else ++it;
        }
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[vector] hand erase  " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[vector] New Size " << vec.size());


        BOOST_TEST_MESSAGE("---remove if + erase ---");
        Start = std::chrono::high_resolution_clock::now();
        auto NewEnd=std::remove_if(vec2.begin(),vec2.end(), [](int i){ return i<0;});
        BOOST_TEST_MESSAGE("[deque]  Size after removeif " << vec2.size());
        vec2.erase(NewEnd,vec2.end());
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[deque] remove+erase" << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[deque] New Size " << vec2.size());
      }




    }






    BOOST_AUTO_TEST_CASE(list)
    {
      const int NBPUSH = 100'000'000;
      { //pushback
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        std::list<int> list;
        BOOST_TEST_MESSAGE("[list] sizeof " << sizeof(list) << " BYTES");
        for (int i = NBPUSH; i > 0; i--)
          list.push_back(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[list] PushBack " << time_span.count() << "s");

      }

      { //pushfront
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        std::list<int> list;
        for (int i = NBPUSH; i > 0; i--)
          list.push_front(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[list] PushFront " << time_span.count() << "s");

      }

    }

    BOOST_AUTO_TEST_CASE(list_find)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---FIND 10 MULT Problem Size =" << NBPUSH);

        std::list<int> vec;

        for (int i = NBPUSH; i > 0; i--)
          vec.push_back(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        for(int j=0;j<NBPUSH;j+=10)
        {
          NbEven+=std::find(vec.begin(), vec.end(),j)!=vec.end() ? 1 :0;
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[list] FIND 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[list] NbEven " << NbEven);
      }

      { // find  type
        const int NBFIND = 1'000'000;
        const int SIZEVEC = 4;
        BOOST_TEST_MESSAGE("---FIND EVEN Nb FIND LOOP =" << NBFIND<<" in Container of "<<SIZEVEC<<" elements" );

        std::list<int> vec;

        for (int i = SIZEVEC; i > 0; i--)
          vec.push_back(i);
        auto Start = std::chrono::high_resolution_clock::now();

        size_t NbEven=0;
        for (int i = NBFIND; i > 0; i--)
          for(int j=0;j<SIZEVEC;j+=2)
          {
            NbEven+=std::find(vec.begin(), vec.end(),j)!=vec.end() ? 1 :0;
          }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[list] Find Even in Small VEC " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[list] NbEven " << NbEven);
      }


    }




    BOOST_AUTO_TEST_CASE(list_insert)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---INSERT  Problem Size =" << NBPUSH);

        std::list<int> vec;
        //

        //
        for (int i = NBPUSH; i > 0; i--)
          vec.push_back(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        auto insertpos=std::next(vec.begin(), (NBPUSH/2));
        for(int j=0;j<NBPUSH;j+=10)
        {
          vec.insert(insertpos,-j);
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[list] INSERT 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[list] New Size " << vec.size());


        std::list<int> vec2(vec);
        BOOST_TEST_MESSAGE("[list] Vec2 New Size " << vec2.size());



        BOOST_TEST_MESSAGE("---hand erase ----");
        Start = std::chrono::high_resolution_clock::now();
        auto it=vec.begin();
        while(it!=vec.end())
        {
          if(*it<0)
            it=vec.erase(it);
          else ++it;
        }
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[list] hand erase  " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[list] New Size " << vec.size());


        BOOST_TEST_MESSAGE("---remove if + erase ---");
        Start = std::chrono::high_resolution_clock::now();
        auto NewEnd=std::remove_if(vec2.begin(),vec2.end(), [](int i){ return i<0;});
        BOOST_TEST_MESSAGE("[list]  Size after removeif " << vec2.size());
        vec2.erase(NewEnd,vec2.end());
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[list] remove+erase" << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[list] New Size " << vec2.size());
      }




    }

    BOOST_AUTO_TEST_CASE(set)
    {

      int NBPUSH = 10'000'000;
      BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
      { //pushback
        auto Start = std::chrono::high_resolution_clock::now();
        std::set<int> set;
        BOOST_TEST_MESSAGE("[set] sizeof " << sizeof(set) << " BYTES");
        for (int i = NBPUSH; i > 0; i--)
          set.insert(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[set] no hint " << time_span.count() << "s");

      }

      {
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        std::set<int> set;
        auto hint_it = set.begin();
        for (int i = NBPUSH; i > 0; i--)
          hint_it = set.insert(hint_it, i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[set] with hint " << time_span.count() << "s");

      }

      NBPUSH = 100'000;
      BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
      { //pushback
        auto Start = std::chrono::high_resolution_clock::now();
        std::set<int> set;
        for (int i = NBPUSH; i > 0; i--)
          set.insert(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[set] no hint " << time_span.count() << "s");

      }

      {
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        std::set<int> set;
        auto hint_it = set.begin();
        for (int i = NBPUSH; i > 0; i--)
          hint_it = set.insert(hint_it, i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[set] with hint " << time_span.count() << "s");

      }

      NBPUSH = 1'000;
      BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
      { //pushback
        auto Start = std::chrono::high_resolution_clock::now();
        std::set<int> set;
        for (int i = NBPUSH; i > 0; i--)
          set.insert(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[set] no hint " << time_span.count() << "s");

      }

      {
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        std::set<int> set;
        auto hint_it = set.begin();
        for (int i = NBPUSH; i > 0; i--)
          hint_it = set.insert(hint_it, i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[set] with hint " << time_span.count() << "s");

      }

      NBPUSH = 100;
      BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
      { //pushback
        auto Start = std::chrono::high_resolution_clock::now();
        std::set<int> set;
        for (int i = NBPUSH; i > 0; i--)
          set.insert(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[set] no hint " << time_span.count() << "s");

      }

      {
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        std::set<int> set;
        auto hint_it = set.begin();
        for (int i = NBPUSH; i > 0; i--)
          hint_it = set.insert(hint_it, i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[set] with hint " << time_span.count() << "s");

      }

    }



    BOOST_AUTO_TEST_CASE(set_find)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---FIND 10 MULT Problem Size =" << NBPUSH);

        std::set<int> set;

        auto hint_it = set.begin();
        for (int i = NBPUSH; i > 0; i--)
          hint_it = set.insert(hint_it, i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        for(int j=0;j<NBPUSH;j+=10)
        {
          NbEven+=set.find(j)!=set.end() ? 1 :0;
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[set] FIND 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[set] NbEven " << NbEven);
      }

      { // find  type
        const int NBFIND = 1'000'000;
        const int SIZEVEC = 4;
        BOOST_TEST_MESSAGE("---FIND EVEN Nb FIND LOOP =" << NBFIND<<" in Container of "<<SIZEVEC<<" elements" );

        std::set<int> set;

        auto hint_it = set.begin();
        for (int i = SIZEVEC; i > 0; i--)
          hint_it = set.insert(hint_it, i);


        auto Start = std::chrono::high_resolution_clock::now();

        size_t NbEven=0;
        for (int i = NBFIND; i > 0; i--)
          for(int j=0;j<SIZEVEC;j+=2)
          {
            NbEven+= set.find(j)!=set.end() ? 1 :0;
          }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[set] Find Even in Small VEC " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[set] NbEven " << NbEven);
      }


    }



    BOOST_AUTO_TEST_CASE(set_insert)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---INSERT  Problem Size =" << NBPUSH);

        std::set<int> vec;
        //

        //
        for (int i = NBPUSH; i > 0; i--)
          vec.insert(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        auto insertpos=std::next(vec.begin(), (NBPUSH/2));
        for(int j=0;j<NBPUSH;j+=10)
        {
          vec.insert(insertpos,-j);
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[set] INSERT 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[set] New Size " << vec.size());


        std::set<int> vec2(vec);
        BOOST_TEST_MESSAGE("[set] Vec2 New Size " << vec2.size());



        BOOST_TEST_MESSAGE("---hand erase ----");
        Start = std::chrono::high_resolution_clock::now();
        auto it=vec.begin();
        while(it!=vec.end())
        {
          if(*it<0)
            it=vec.erase(it);
          else ++it;
        }
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[set] hand erase  " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[set] New Size " << vec.size());


      }




    }





    BOOST_AUTO_TEST_CASE(uset)
    {
      int NBPUSH = 10'000'000;
      BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
      { //pushback
        auto Start = std::chrono::high_resolution_clock::now();
        std::unordered_set<int> uset;
        BOOST_TEST_MESSAGE("[uset] sizeof " << sizeof(uset) << " BYTES");
        for (int i = NBPUSH; i > 0; i--)
          uset.insert(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[uset]  " << time_span.count() << "s");

      }

      NBPUSH = 100'000;
      BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
      { //pushback
        auto Start = std::chrono::high_resolution_clock::now();
        std::unordered_set<int> uset;
        for (int i = NBPUSH; i > 0; i--)
          uset.insert(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[uset]  " << time_span.count() << "s");

      }
      NBPUSH = 1'000;
      BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
      { //pushback
        auto Start = std::chrono::high_resolution_clock::now();
        std::unordered_set<int> uset;
        for (int i = NBPUSH; i > 0; i--)
          uset.insert(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[uset]  " << time_span.count() << "s");

      }
      NBPUSH = 1'000;
      BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
      { //pushback
        auto Start = std::chrono::high_resolution_clock::now();
        std::unordered_set<int> uset;
        for (int i = NBPUSH; i > 0; i--)
          uset.insert(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[uset]  " << time_span.count() << "s");

      }
      NBPUSH = 100;
      BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
      { //pushback
        auto Start = std::chrono::high_resolution_clock::now();
        std::unordered_set<int> uset;
        for (int i = NBPUSH; i > 0; i--)
          uset.insert(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[uset]  " << time_span.count() << "s");

      }
    }

    BOOST_AUTO_TEST_CASE(uset_find)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---FIND 10 MULT Problem Size =" << NBPUSH);

        std::unordered_set<int> set;
        for (int i = NBPUSH; i > 0; i--)
          set.insert( i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        for(int j=0;j<NBPUSH;j+=10)
        {
          NbEven+=set.find(j)!=set.end() ? 1 :0;
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[uset] FIND 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[uset] NbEven " << NbEven);
      }

      { // find  type
        const int NBFIND = 1'000'000;
        const int SIZEVEC = 4;
        BOOST_TEST_MESSAGE("---FIND EVEN Nb FIND LOOP =" << NBFIND<<" in Container of "<<SIZEVEC<<" elements" );

        std::unordered_set<int> set;
        for (int i = SIZEVEC; i > 0; i--)
          set.insert( i);


        auto Start = std::chrono::high_resolution_clock::now();

        size_t NbEven=0;
        for (int i = NBFIND; i > 0; i--)
          for(int j=0;j<SIZEVEC;j+=2)
          {
            NbEven+= set.find(j)!=set.end() ? 1 :0;
          }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[uset] Find Even in Small VEC " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[uset] NbEven " << NbEven);
      }


    }


    BOOST_AUTO_TEST_CASE(uset_insert)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---INSERT  Problem Size =" << NBPUSH);

        std::unordered_set<int> vec;
        //

        //
        for (int i = NBPUSH; i > 0; i--)
          vec.insert(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        auto insertpos=std::next(vec.begin(), (NBPUSH/2));
        for(int j=0;j<NBPUSH;j+=10)
        {
          vec.insert(insertpos,-j);
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[unordered_set] INSERT 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[unordered_set] New Size " << vec.size());


        std::unordered_set<int> vec2(vec);
        BOOST_TEST_MESSAGE("[unordered_set] Vec2 New Size " << vec2.size());



        BOOST_TEST_MESSAGE("---hand erase ----");
        Start = std::chrono::high_resolution_clock::now();
        auto it=vec.begin();
        while(it!=vec.end())
        {
          if(*it<0)
            it=vec.erase(it);
          else ++it;
        }
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[unordered_set] hand erase  " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[unordered_set] New Size " << vec.size());


//        BOOST_TEST_MESSAGE("---remove if + erase ---");
//        Start = std::chrono::high_resolution_clock::now();
//        auto NewEnd=std::remove_if(vec2.begin(),vec2.end(), [](int i){ return i<0;});
//        BOOST_TEST_MESSAGE("[list]  Size after removeif " << vec2.size());
//        vec2.erase(NewEnd,vec2.end());
//        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
//
//        BOOST_TEST_MESSAGE("[unordered_set] remove+erase" << time_span.count() << "s");
//        BOOST_TEST_MESSAGE("[unordered_set] New Size " << vec2.size());
      }




    }

// BOOST_AUTO_TEST_CASE(devector)
// {
//    const int NBPUSH=100'000'000;
//    { //pushback
//     BOOST_TEST_MESSAGE("---Problem Size =" <<NBPUSH);
//       auto Start = std::chrono::high_resolution_clock::now();
//       boost::container::devector<int> devector;
//       for(int i=NBPUSH;i>0;i--)
//       devector.push_back(i);            

//       std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
//       BOOST_TEST_MESSAGE("[devector] PushBack " << time_span.count() << "s");

//    }

//    { //pushfront
//       auto Start = std::chrono::high_resolution_clock::now();
//       boost::container::devector devector;
//       for(int i=NBPUSH;i>0;i--)
//       devector.push_front(i);            

//       std::chrono::duration<float> time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
//       BOOST_TEST_MESSAGE("[devector] PushFront " << time_span.count() << "s");

//    }



// }

    BOOST_AUTO_TEST_CASE(stable_vector)
    {
      const int NBPUSH = 100'000'000;
      { //pushback
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        ::boost::container::stable_vector<int> svector;
        BOOST_TEST_MESSAGE("[stable_vector] sizeof " << sizeof(svector) << " BYTES");
        for (int i = NBPUSH; i > 0; i--)
          svector.push_back(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[stable_vector] PushBack " << time_span.count() << "s");

      }

    }

    BOOST_AUTO_TEST_CASE(stablevec_find)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---FIND 10 MULT Problem Size =" << NBPUSH);

        ::boost::container::stable_vector<int> vec;

        for (int i = NBPUSH; i > 0; i--)
          vec.push_back(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        for(int j=0;j<NBPUSH;j+=10)
        {
          NbEven+=std::find(vec.begin(), vec.end(),j)!=vec.end() ? 1 :0;
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[stable_vector] FIND 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[stable_vector] NbEven " << NbEven);
      }

      { // find  type
        const int NBFIND = 1'000'000;
        const int SIZEVEC = 4;
        BOOST_TEST_MESSAGE("---FIND EVEN Nb FIND LOOP =" << NBFIND<<" in Container of "<<SIZEVEC<<" elements" );

        ::boost::container::stable_vector<int> vec;

        for (int i = SIZEVEC; i > 0; i--)
          vec.push_back(i);
        auto Start = std::chrono::high_resolution_clock::now();

        size_t NbEven=0;
        for (int i = NBFIND; i > 0; i--)
          for(int j=0;j<SIZEVEC;j+=2)
          {
            NbEven+=std::find(vec.begin(), vec.end(),j)!=vec.end() ? 1 :0;
          }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[stable_vector] Find Even in Small VEC " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[stable_vector] NbEven " << NbEven);
      }


    }




    BOOST_AUTO_TEST_CASE(stablevector_insert)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---INSERT  Problem Size =" << NBPUSH);

        ::boost::container::stable_vector<int> vec;
        //

        //
        for (int i = NBPUSH; i > 0; i--)
          vec.push_back(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        auto insertpos=std::next(vec.begin(), (NBPUSH/2));
        for(int j=0;j<NBPUSH;j+=10)
        {
          vec.insert(insertpos,-j);
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[stable_vector] INSERT 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[stable_vector] New Size " << vec.size());


        ::boost::container::stable_vector<int> vec2(vec);
        BOOST_TEST_MESSAGE("[stable_vector] Vec2 New Size " << vec2.size());



        BOOST_TEST_MESSAGE("---hand erase ----");
        Start = std::chrono::high_resolution_clock::now();
        auto it=vec.begin();
        while(it!=vec.end())
        {
          if(*it<0)
            it=vec.erase(it);
          else ++it;
        }
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[stable_vector] hand erase  " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[stable_vector] New Size " << vec.size());


        BOOST_TEST_MESSAGE("---remove if + erase ---");
        Start = std::chrono::high_resolution_clock::now();
        auto NewEnd=std::remove_if(vec2.begin(),vec2.end(), [](int i){ return i<0;});
        BOOST_TEST_MESSAGE("[stable_vector]  Size after removeif " << vec2.size());
        vec2.erase(NewEnd,vec2.end());
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[stable_vector] remove+erase" << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[stable_vector] New Size " << vec2.size());
      }




    }



    BOOST_AUTO_TEST_CASE(static_vector)
    {

      { //pushback
        constexpr int NBPUSH = 1'000'000;
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        ::boost::container::static_vector<int, NBPUSH> svector;
        BOOST_TEST_MESSAGE("[static_vector] sizeof " << sizeof(svector) << " BYTES");
        for (int i = NBPUSH; i > 0; i--)
          svector.push_back(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[static_vector] PushBack " << time_span.count() << "s");

        BOOST_TEST_MESSAGE("Try Extra pushes (/!\\ SHOULD THROW) :" << NBPUSH);
        Start = std::chrono::high_resolution_clock::now();

        //Show it will TH
        for (int i = NBPUSH; i > 0; i--)
          svector.push_back(i);
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[static_vector] PushBack " << time_span.count() << "s");
      }

    }

    BOOST_AUTO_TEST_CASE(static_vector_find)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---FIND 10 MULT Problem Size =" << NBPUSH);

        ::boost::container::static_vector<int, NBPUSH> vec;

        for (int i = NBPUSH; i > 0; i--)
          vec.push_back(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        for(int j=0;j<NBPUSH;j+=10)
        {
          NbEven+=std::find(vec.begin(), vec.end(),j)!=vec.end() ? 1 :0;
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[static_vector] FIND 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[static_vector] NbEven " << NbEven);
      }

      { // find  type
        const int NBFIND = 1'000'000;
        const int SIZEVEC = 4;
        BOOST_TEST_MESSAGE("---FIND EVEN Nb FIND LOOP =" << NBFIND<<" in Container of "<<SIZEVEC<<" elements" );

        ::boost::container::static_vector<int, SIZEVEC> vec;

        for (int i = SIZEVEC; i > 0; i--)
          vec.push_back(i);
        auto Start = std::chrono::high_resolution_clock::now();

        size_t NbEven=0;
        for (int i = NBFIND; i > 0; i--)
          for(int j=0;j<SIZEVEC;j+=2)
          {
            NbEven+=std::find(vec.begin(), vec.end(),j)!=vec.end() ? 1 :0;
          }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[static_vector] Find Even in Small VEC " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[static_vector] NbEven " << NbEven);
      }


    }





    BOOST_AUTO_TEST_CASE(staticvector_insert)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---INSERT  Problem Size =" << NBPUSH);

        ::boost::container::static_vector<int,2*NBPUSH> vec;
        //

        //
        for (int i = NBPUSH; i > 0; i--)
          vec.push_back(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        auto insertpos=std::next(vec.begin(), (NBPUSH/2));
        for(int j=0;j<NBPUSH;j+=10)
        {
          vec.insert(insertpos,-j);
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[static_vector] INSERT 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[static_vector] New Size " << vec.size());


        ::boost::container::static_vector<int,2*NBPUSH> vec2(vec);
        BOOST_TEST_MESSAGE("[static_vector] Vec2 New Size " << vec2.size());



        BOOST_TEST_MESSAGE("---hand erase ----");
        Start = std::chrono::high_resolution_clock::now();
        auto it=vec.begin();
        while(it!=vec.end())
        {
          if(*it<0)
            it=vec.erase(it);
          else ++it;
        }
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[static_vector] hand erase  " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[static_vector] New Size " << vec.size());


        BOOST_TEST_MESSAGE("---remove if + erase ---");
        Start = std::chrono::high_resolution_clock::now();
        auto NewEnd=std::remove_if(vec2.begin(),vec2.end(), [](int i){ return i<0;});
        BOOST_TEST_MESSAGE("[static_vector]  Size after removeif " << vec2.size());
        vec2.erase(NewEnd,vec2.end());
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[static_vector] remove+erase" << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[static_vector] New Size " << vec2.size());
      }

    }


    BOOST_AUTO_TEST_CASE(small_vector)
    {

      { //pushback
        constexpr int NBPUSH = 1'000'000;
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH << " Pre Allocated size : " << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        ::boost::container::small_vector<int, NBPUSH> svector;
        BOOST_TEST_MESSAGE("[small_vector] sizeof " << sizeof(svector) << "BYTES");
        for (int i = NBPUSH; i > 0; i--)
          svector.push_back(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[small_vector] PushBack " << time_span.count() << "s");

      }

      {
        const int NBPUSH = 100'000'000;
        const int PreAlloc = 1000;
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH << " Pre Allocated size : " << PreAlloc);

        auto Start = std::chrono::high_resolution_clock::now();
        ::boost::container::small_vector<int, PreAlloc> svector;
        BOOST_TEST_MESSAGE("[small_vector] sizeof " << sizeof(svector) << " BYTES");
        for (int i = NBPUSH; i > 0; i--)
          svector.push_back(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[small_vector] PushBack " << time_span.count() << "s");
      }

    }

    BOOST_AUTO_TEST_CASE(small_vector_find)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---FIND 10 MULT Problem Size =" << NBPUSH);

        ::boost::container::small_vector<int, NBPUSH> vec;

        for (int i = NBPUSH; i > 0; i--)
          vec.push_back(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        for(int j=0;j<NBPUSH;j+=10)
        {
          NbEven+=std::find(vec.begin(), vec.end(),j)!=vec.end() ? 1 :0;
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[small_vector] FIND 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[small_vector] NbEven " << NbEven);
      }

      { // find  type
        const int NBFIND = 1'000'000;
        const int SIZEVEC = 4;
        BOOST_TEST_MESSAGE("---FIND EVEN Nb FIND LOOP =" << NBFIND<<" in Container of "<<SIZEVEC<<" elements" );

        ::boost::container::small_vector<int, SIZEVEC> vec;

        for (int i = SIZEVEC; i > 0; i--)
          vec.push_back(i);
        auto Start = std::chrono::high_resolution_clock::now();

        size_t NbEven=0;
        for (int i = NBFIND; i > 0; i--)
          for(int j=0;j<SIZEVEC;j+=2)
          {
            NbEven+=std::find(vec.begin(), vec.end(),j)!=vec.end() ? 1 :0;
          }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[small_vector] Find Even in Small VEC " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[small_vector] NbEven " << NbEven);
      }


    }

    BOOST_AUTO_TEST_CASE(smallvector_insert)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---INSERT  Problem Size =" << NBPUSH);

        ::boost::container::small_vector<int,2*NBPUSH> vec;
        //

        //
        for (int i = NBPUSH; i > 0; i--)
          vec.push_back(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        auto insertpos=std::next(vec.begin(), (NBPUSH/2));
        for(int j=0;j<NBPUSH;j+=10)
        {
          vec.insert(insertpos,-j);
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[small_vector] INSERT 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[small_vector] New Size " << vec.size());


        ::boost::container::small_vector<int,2*NBPUSH> vec2(vec);
        BOOST_TEST_MESSAGE("[small_vector] Vec2 New Size " << vec2.size());



        BOOST_TEST_MESSAGE("---hand erase ----");
        Start = std::chrono::high_resolution_clock::now();
        auto it=vec.begin();
        while(it!=vec.end())
        {
          if(*it<0)
            it=vec.erase(it);
          else ++it;
        }
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[small_vector] hand erase  " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[small_vector] New Size " << vec.size());


        BOOST_TEST_MESSAGE("---remove if + erase ---");
        Start = std::chrono::high_resolution_clock::now();
        auto NewEnd=std::remove_if(vec2.begin(),vec2.end(), [](int i){ return i<0;});
        BOOST_TEST_MESSAGE("[small_vector]  Size after removeif " << vec2.size());
        vec2.erase(NewEnd,vec2.end());
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[small_vector] remove+erase" << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[small_vector] New Size " << vec2.size());
      }

    }







    BOOST_AUTO_TEST_CASE(flat_set)
    {

      int NBPUSH = 100'000;
      BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
      { //pushback

        ::boost::container::flat_set<int> set;
        BOOST_TEST_MESSAGE("[stable_vector] sizeof " << sizeof(set) << " BYTES");
        auto Start = std::chrono::high_resolution_clock::now();

        for (int i = NBPUSH; i > 0; i--)
          set.insert(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[flat_set] no hint " << time_span.count() << "s");

      }

      {
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        ::boost::container::flat_set<int> set;
        auto hint_it = set.begin();
        for (int i = NBPUSH; i > 0; i--)
          hint_it = set.insert(hint_it, i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[flat_set] with hint " << time_span.count() << "s");

      }

      NBPUSH = 1'000;
      BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
      { //pushback
        auto Start = std::chrono::high_resolution_clock::now();
        ::boost::container::flat_set<int> set;
        for (int i = NBPUSH; i > 0; i--)
          set.insert(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[flat_set] no hint " << time_span.count() << "s");

      }

      {
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        ::boost::container::flat_set<int> set;
        auto hint_it = set.begin();
        for (int i = NBPUSH; i > 0; i--)
          hint_it = set.insert(hint_it, i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[flat_set] with hint " << time_span.count() << "s");

      }

      NBPUSH = 100;
      BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
      { //pushback
        auto Start = std::chrono::high_resolution_clock::now();
        ::boost::container::flat_set<int> set;
        for (int i = NBPUSH; i > 0; i--)
          set.insert(i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[flat_set] no hint " << time_span.count() << "s");

      }

      {
        BOOST_TEST_MESSAGE("---Problem Size =" << NBPUSH);
        auto Start = std::chrono::high_resolution_clock::now();
        ::boost::container::flat_set<int> set;
        auto hint_it = set.begin();
        for (int i = NBPUSH; i > 0; i--)
          hint_it = set.insert(hint_it, i);

        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[flat_set] with hint " << time_span.count() << "s");

      }

    }



    BOOST_AUTO_TEST_CASE(flatset_find)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---FIND 10 MULT Problem Size =" << NBPUSH);

        ::boost::container::flat_set<int> set;

        auto hint_it = set.begin();
        for (int i = NBPUSH; i > 0; i--)
          hint_it = set.insert(hint_it, i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        for(int j=0;j<NBPUSH;j+=10)
        {
          NbEven+=set.find(j)!=set.end() ? 1 :0;
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[flat_set] FIND 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[flat_set] NbEven " << NbEven);
      }

      { // find  type
        const int NBFIND = 1'000'000;
        const int SIZEVEC = 4;
        BOOST_TEST_MESSAGE("---FIND EVEN Nb FIND LOOP =" << NBFIND<<" in Container of "<<SIZEVEC<<" elements" );

        ::boost::container::flat_set<int> set;

        auto hint_it = set.begin();
        for (int i = SIZEVEC; i > 0; i--)
          hint_it = set.insert(hint_it, i);


        auto Start = std::chrono::high_resolution_clock::now();

        size_t NbEven=0;
        for (int i = NBFIND; i > 0; i--)
          for(int j=0;j<SIZEVEC;j+=2)
          {
            NbEven+= set.find(j)!=set.end() ? 1 :0;
          }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[flat_set] Find Even in Small VEC " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[flat_set] NbEven " << NbEven);
      }


    }




    BOOST_AUTO_TEST_CASE(flatset_insert)
    {
      ///----------------------------
      //------ FIND -----------------
      { // find  type
        const int NBPUSH = 250'000;
        BOOST_TEST_MESSAGE("---INSERT  Problem Size =" << NBPUSH);

        ::boost::container::flat_set<int> vec;
        //

        //
        for (int i = NBPUSH; i > 0; i--)
          vec.insert(i);

        auto Start = std::chrono::high_resolution_clock::now();
        size_t NbEven=0;
        //auto insertpos=std::next(vec.begin(), (NBPUSH/2));
        for(int j=0;j<NBPUSH;j+=10)
        {
          vec.insert(-j);
        }


        std::chrono::duration<float>
            time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[flat_set] INSERT 10 MULT  in BIG " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[flat_set] New Size " << vec.size());


        ::boost::container::flat_set<int> vec2(vec);
        BOOST_TEST_MESSAGE("[flat_set] Vec2 New Size " << vec2.size());



        BOOST_TEST_MESSAGE("---hand erase ----");
        Start = std::chrono::high_resolution_clock::now();
        auto it=vec.begin();
        while(it!=vec.end())
        {
          if(*it<0)
            it=vec.erase(it);
          else ++it;
        }
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);
        BOOST_TEST_MESSAGE("[flat_set] hand erase  " << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[flat_set] New Size " << vec.size());


        BOOST_TEST_MESSAGE("---remove if + erase ---");
        Start = std::chrono::high_resolution_clock::now();
        auto NewEnd=std::remove_if(vec2.begin(),vec2.end(), [](int i){ return i<0;});
        BOOST_TEST_MESSAGE("[flat_set]  Size after removeif " << vec2.size());
        vec2.erase(NewEnd,vec2.end());
        time_span = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - Start);

        BOOST_TEST_MESSAGE("[flat_set] remove+erase" << time_span.count() << "s");
        BOOST_TEST_MESSAGE("[flat_set] New Size " << vec2.size());
      }

    }

//BOOST_AUTO_TEST_SUITE_END()



// No more polar bears should die now
