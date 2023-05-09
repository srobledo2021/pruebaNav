#ifndef NAVIGATE_BT__INTRODUCTION_HPP_
#define NAVIGATE_BT__INTRODUCTION_HPP_

#include <string>

#include "sound_play.hpp"
#include "behaviortree_cpp_v3/behavior_tree.h"
#include "behaviortree_cpp_v3/bt_factory.h"
#include "gb_dialog/DialogInterface.hpp"

namespace navigate_bt
{

class Introduction : public BT::ActionNodeBase
{
public:
  explicit Introduction(
    const std::string & xml_tag_name,
    const BT::NodeConfiguration & conf);


  void halt();
  BT::NodeStatus tick();

  static BT::PortsList providedPorts()
  {
    return{
      BT::InputPort<std::string>("nameID")
      };
  }

private:
  rclcpp::Node::SharedPtr node_;
  gb_dialog::DialogInterface dialog_;
  rclcpp::Time  start_time_;
  std::string name_ = "";
};

}  // namespace navigate_bt

#endif  // INTRODUCTION_BT__REQUESTNAME_HPP_