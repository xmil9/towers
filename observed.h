//
// Oct-2020, Michael Lindner
// MIT license
//
#pragma once
#include <functional>
#include <string>
#include <vector>


///////////////////

// Base class for event data.
struct ObservedEventData
{
 protected:
   ~ObservedEventData() = default;
};


///////////////////

template <typename Source> class Observed
{
 public:
   using Callback =
      std::function<void(Source&, std::string_view, const ObservedEventData& data)>;

   void addObserver(Callback cb);
   void removeObserver(Callback cb);
   void notify(Source& src, std::string_view event, const ObservedEventData& data);

 private:
   std::vector<Callback> m_observers;
};


template <typename Source> void Observed<Source>::addObserver(Callback cb)
{
   m_observers.push_back(cb);
}


template <typename Source> void Observed<Source>::removeObserver(Callback cb)
{
   const auto pos = std::find(std::begin(m_observers), std::end(m_observers), cb);
   if (pos != m_observers.end())
      m_observers.erase(pos);
}


template <typename Source>
void Observed<Source>::notify(Source& src, std::string_view event,
                              const ObservedEventData& data)
{
   for (auto& cb : m_observers)
      cb(src, event, data);
}
