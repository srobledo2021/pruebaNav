#include <string>
#include <iostream>
#include <stdio.h>

#include "navigate_bt/Introduction.hpp"
#include "gb_dialog/DialogInterface.hpp"
#include "behaviortree_cpp_v3/behavior_tree.h"

#include "rclcpp/rclcpp.hpp"

using namespace std::placeholders;

using namespace std::chrono_literals;

namespace navigate_bt
{

Introduction::Introduction(
  const std::string & xml_tag_name,
  const BT::NodeConfiguration & conf)
: BT::ActionNodeBase(xml_tag_name, conf)
{
  config().blackboard->get("node",node_);
}


BT::NodeStatus
Introduction::tick()
{
  rclcpp::spin_some(dialog_.get_node_base_interface());
  getInput("nameID", name_);
  dialog_.speak("Nice to meet you "+ name_ + "Follow me. I will give you a seat");

  return BT::NodeStatus::SUCCESS;

}  // namespace navigate_bt

}

#include "behaviortree_cpp_v3/bt_factory.h"

BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<navigate_bt::Introduction>("Introduction");
}
