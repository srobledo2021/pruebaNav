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
    std::bind(&RequestName::RequestNameCB, this, _1),"request_name");
  config().blackboard->get("node",node_);
}


void RequestName::RequestNameCB(dialogflow_ros2_interfaces::msg::DialogflowResult result)
  {
    name_ =result.parameters[0].value[0];
  }

void
RequestName::halt()
{
}
BT::NodeStatus
RequestName::tick()
{
  if (status() == BT::NodeStatus::IDLE) {
    start_time_ = node_->now();
    dialog_.speak("Howdy! What is your name?");
  }
  rclcpp::spin_some(dialog_.get_node_base_interface());

  if(name_ == "" && node_->now() - start_time_ > 10s){
    RCLCPP_INFO(node_->get_logger(),"Reboot the system");
    listening_ = false;
    return BT::NodeStatus::FAILURE;
  }

  if(node_->now() - start_time_  < 3s){
    return BT::NodeStatus::RUNNING;
  }

  if(!listening_){
    listening_ = true;
    dialog_.listen();
  }

  if(name_== ""){
    return BT::NodeStatus::RUNNING;
  }

  dialog_.speak("Hello "+ name_ + "Nice to meet you. You are really welcomed!");

  setOutput("nameID",name_);

  name_="";
  listening_ = false;

  return BT::NodeStatus::SUCCESS;

}  // namespace navigate_bt

}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<navigate_bt::RequestName>("request_name");
}
