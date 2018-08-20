import React, { Component } from 'react';
import './App.css';
import Eos from "eosjs";

const eos = Eos({
  keyProvider: [
    "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3",
    "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3"
  ]
});

const pubkey = 'EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV'

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {isToggleOn: true, inputName: ""};
    

    // This binding is necessary to make `this` work in the callback
    this.handleClick = this.handleClick.bind(this);
    this.onChange = this.onChange.bind(this);
  }
	
  transCur() {

   eos.transaction({
         actions: [
           {
             account: "eosio.token",
             name: "transfer",
             authorization: [
               {
                 actor: "user2",
                 permission: "active"
               }
             ],       

             data: {
              from: "user2",
              to: "user1",
              quantity: `5.0000 TOK`,
              memo: "Roflan buldiga"
            } 
           }
         ]
        });
     
  }


  createAccount(accName) {
    eos.transaction(tr => {
      tr.newaccount({
        creator: 'eosio',
        name: accName,
        owner: pubkey,
        active: pubkey
      })
    
      tr.buyrambytes({
        payer: 'eosio',
        receiver: accName,
        bytes: 8192
      })
    
      tr.delegatebw({
        from: 'eosio',
        receiver: accName,
        stake_net_quantity: '10.0000 SYS',
        stake_cpu_quantity: '10.0000 SYS',
        transfer: 0
      })
    }).then(()=>{console.log("New account " + accName + " created")}).catch(e => {console.log(e)});
  }

  handleClick() {
    eos.contract('user5').then(contract => 
          contract.createuser("lol1",
            {authorization : "lol1"}
          )).catch(e => {console.log(e)});

    eos.getTableRows({json:true, scope: 'user5', code: 'user5', table: 'employee', limit:1, lower_bound: 0, upper_bound: 1})
                                  .then(res => { let row = res.rows[0]; console.log(typeof(row));}).catch(e => {console.log(e)});
    console.log(this.state.inputName)
    this.createAccount(this.state.inputName);
    this.setState(prevState => ({
      isToggleOn: !prevState.isToggleOn
    }));
  }

  onChange(e) {
    var val = e.target.value;
    this.setState({inputName: val});
  }


  render() {
    return (
      <div>
        <form onSumbit>
          <p>
            <label> Name: </label> <br />
            <input type = "text" value = {this.state.name} onChange = {this.onChange} />
          </p>
        </form>
        <button onClick={this.handleClick}>
          {this.state.isToggleOn ? 'ON' : 'OFF'}
       </button>
      </div> 
       
    );
  }
}

export default App;
