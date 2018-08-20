#include <eosiolib/eosio.hpp>

#include <string>
#include <vector>

class event_manager : public eosio::contract {

protected:

   // izvrashyayus' kak mogu
   typedef uint16_t SKILL;
   #define CPP 0
   #define PYTHON 1
   #define ML 2
   #define _SKILLS_NUMBER_ 3 
public:

    event_manager( account_name self ) : contract(self) {}

    /// @abi table 
    struct employee
    {
        account_name id;
        std::vector<uint64_t> SkillTable;
        uint8_t all_events;  // num of all events

	employee( void )
	{
          eosio::print("employee constructor started\n");
	  for (int i = 0; i < _SKILLS_NUMBER_; i++)
	    SkillTable.push_back(0);
	  all_events = 0;
          eosio::print("Skill table is set to zero - \n", eosio::name{id});
	}

        auto primary_key() const 
	{ 
          eosio::print("employee pr_k\n"); 
	  return id; 
	}
        EOSLIB_SERIALIZE( employee, (id)(SkillTable)(all_events))
    };

    typedef eosio::multi_index<N(employee), employee> _employee;

    /// @abi table 
    struct event
    {
        uint64_t id;   
        uint64_t tok;  // total token number
        SKILL skill;   
        account_name organizer;
        std::string date;
        std::vector<account_name> part;  // list of participants

        auto primary_key() const {return id;}

        EOSLIB_SERIALIZE(event, (id)(tok)(skill)(organizer)(date)(part))
    }; /* End of event struct */

    typedef eosio::multi_index<N(event), event> events;

    /// @abi action
    void create( uint64_t id, const account_name &org, uint16_t skill, uint16_t tok ); // create new event (only by contract owner)

    /// @abi action
    void createuser( const account_name &id ); // (user only can registrate himself)
	
    /// @abi action
    void sendtok( const account_name &org, const account_name &_to, uint64_t tok, uint64_t id  );  // send tokens to current user from event storage (only by oraganizer)
	
    /// @abi action
    void addpart( const account_name &org, const account_name &us, uint64_t id );  // add participant to cuurent event (only by oraganizer)
};

