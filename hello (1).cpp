#include <algorithm>
#include "hello.hpp"

using namespace eosio;

/// new event create
///                         id of event|     organzer           | skill of event| num of tokens
void event_manager::create( uint64_t id, const account_name &org, uint16_t skill, uint16_t tok )
{
    print("New event creation started\n");
    require_auth(_self);

    // check if event exists
    events existing_events(_self, _self);
    auto itr = existing_events.find(id);
    eosio_assert(itr == existing_events.end(), "Event already exist");

    // add new event to the table
    existing_events.emplace(_self, [&](auto &e) {
        e.id = id;
        e.organizer = org;
        e.skill = skill;
        e.tok = tok;
    });

    print("New event created\n");
}

/// new user create
void event_manager::createuser( const account_name &id )
{
  print("Create user started\n");	
  require_auth(id);
  _employee existing_employee(_self, _self);

  auto itr = existing_employee.find(id);
  eosio_assert(itr == existing_employee.end(), "User already exists\n");
   
  // add new employee to the table
  existing_employee.emplace(_self, [&](auto &u) {   
    u.id = id;
  });
  print("New user created\n");
}

/// send tokens from event
void event_manager::sendtok( const account_name &org, const account_name &_to, uint64_t tok, uint64_t id )
{
  print("send tok started\n");
  require_auth(org);
  events existing_events(_self, _self);
  auto itr = existing_events.find(id);

  eosio_assert(itr->organizer == org, "Only organizer can share tokens");
  eosio_assert(itr->tok > tok, "Not enough tokens left");	
  eosio_assert(tok > 0, "Tokens should be positive");   
  eosio_assert(itr != existing_events.end(), "Event does not exist");
   
  _employee existing_employee(_self, _self);
  auto itr1 = existing_employee.find(_to);
  eosio_assert(itr1 != existing_employee.end(), "No such employee");
  existing_employee.modify(itr1, org, [=](auto &emp) {
            emp.SkillTable[itr->skill] += tok;   // increase user tokens
  });
  existing_events.modify(itr, org, [=](auto &evn) {
            evn.tok -= tok;  			 // decrease event tokens
  });
  print("send tok ended\n");
}


// add paricipant to the event
void event_manager::addpart( const account_name &org, const account_name &us, uint64_t id )
{
  print("addpart started\n");
  uint8_t flag = 0;
  require_auth(org);
  
  events existing_events(_self, _self);
  auto itr = existing_events.find(id);

  _employee existing_employee(_self, _self);
  auto itr1 = existing_employee.find(us);

  eosio_assert(itr1 != existing_employee.end(), "User not found");
  eosio_assert(itr != existing_events.end(), "Event does not exist");
  eosio_assert(itr->organizer == org, "Only organizer can add paricipants");
  
  // add participant to current event
  existing_events.modify(itr, org, [&](auto &evn) {
            // check if he is already in event
            if (std::find(evn.part.begin(), evn.part.end(), us) == evn.part.end())
            {
              evn.part.push_back(us);
	      print("part added\n");
              flag = 1; 
             
            }
            else
              print("User already in event");
  });  
 // if he wasn't in event list then increase his number of events
 if (flag)
   existing_employee.modify(itr1, org, [&](auto &emp) {
                emp.all_events += 1;
                print("Num of events increased\n");
              });
}



EOSIO_ABI( event_manager, (create)(createuser)(sendtok)(addpart))
