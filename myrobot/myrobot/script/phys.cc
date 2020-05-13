#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physcis.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vectord.hh>

namespace gazebo
{
  class ModelPush : public ModelPlugin
  {
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
      // store the pointer to the model
      this->model = _parent;

      //listen to the update event. This event is broadcast every
      //simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
           std::bind(&ModelPush::OnUpdate, this));
    }

    //Called by the world update start event
    public: void OnUpdate()
    {
      // Apply a smal linear velocity to the model
      this->model->SetLinearVel(ignition::math::Vector3d(.3, 0, 0));
    }

    //pointer to model
    private: physics::ModelPtr model;

    //pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  //register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ModelPush)
}
