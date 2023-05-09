#include <string>
#include <iostream>
#include <stdio.h>

#include "navigate_bt/RequestName.hpp"
#include "gb_dialog/DialogInterface.hpp"
#include "behaviortree_cpp_v3/behavior_tree.h"

#include "rclcpp/rclcpp.hpp"

using namespace std::placeholders;

using namespace std::chrono_literals;

namespace navigate_bt
{

RequestName::RequestName(
  const std::string & xml_tag_name,
  const BT::NodeConfiguration & conf)
: BT::ActionNodeBase(xml_tag_name, conf)
{
  dialog_.registerCallback(
    std::bind(&RequestName::RequestNameCB, this, _1),"RequestName");
  config().blackboard->get("node",node_);

  config().blackboard->get("dialog",dialog_);
}


void RequestName::RequestNameCB(dialogflow_ros2_interfaces::msg::DialogflowResult result)
  {
    RCLCPP_INFO(node_->get_logger(), "[ExampleDF] RequestNameCB: intent [%s]", result.intent.c_str());
    dialog_.speak(result.fulfillment_text);
    name_ = result.parameters[0].value[0];
  }
BT::NodeStatus

RequestName::tick() 
{
  if (status() == BT::NodeStatus::IDLE) {
    start_time_ = node_->now();
    dialog_.speak("Howdy! What is your name?");
  }
  

  // if(!listening_ ){
  //   listening_ = true;
  //   dialog_.listen();
  // }

  if(!listening_ && name_ == "" && node_->now() - start_time_ > 10s){
    listening_ = false;
    RCLCPP_INFO(node_->get_logger(),"Reboot the system" );
    return BT::NodeStatus::FAILURE;
  }

  if (!listening_ && name_ == "" && node_->now() - start_time_ > 3s) {
    return BT::NodeStatus::RUNNING;
  }
  if (!listening_ && dialog_.listen()) {
    listening_ = true;
    return BT::NodeStatus::RUNNING;
  } 

  if(name_== ""){
    return BT::NodeStatus::RUNNING;
  }

  dialog_.speak("Hello "+ name_ + "Nice to meet you. You are really welcomed!");

  setOutput("nameID",name_);

  name_="";
  listening_ = false;

  rclcpp::spin_some(dialog_.get_node_base_interface());
  return BT::NodeStatus::SUCCESS;

}  // namespace navigate_bt

}


#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<navigate_bt::RequestName>("RequestName");
}
