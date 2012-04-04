/***************************************************************************
 *   Copyright (C) 2012 by                                                 *
 *    Martin Biehl <mab@physik3.gwdg.de>                                   *
 *    Guillaume de Chambrier <s0672742@sms.ed.ac.uk>                       *
 *    martius@informatik.uni-leipzig.de                                    *
 *    Timo Nachstedt <nachstedt@physik3.gwdg.de>                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 **************************************************************************/


#ifndef __AMOSII_H
#define __AMOSII_H

#include <selforg/inspectable.h>
#include <ode_robots/oderobot.h>
#include <ode_robots/raysensorbank.h>
#include "spring.h"
#include "amosIIconf.h"
#include <selforg/amosiisensormotordefinition.h>


/**
 * forward declarations
 */
namespace lpzrobots {
    class Primitive;
    class Joint;
    class OneAxisServo;
    class TwoAxisServo;
    class Spring;
    class IRSensor;
    class SpeedSensor;
}

namespace lpzrobots {

    class AmosII : public OdeRobot, public Inspectable {
    public:
  
        static AmosIIConf getDefaultConf(
                double _scale = 1.0,
                bool _useShoulder = 1,
                bool _useFoot = 1,
                bool _useBack = 0);

        /**
         * joint limits differ, density is reduced with scale
         */
        static AmosIIConf getExperimentConf(
                double _scale = 1.0,
                bool _useShoulder = 1,
                bool _useFoot = 1,
                bool _useBack = 0);

        static AmosIIConf getFixedConf(
                double _scale = 1.0,
                bool _useShoulder = 1,
                bool _useFoot = 1,
                bool _useBack = 0);

        static AmosIIConf getFranksDefaultConf(
                double _scale = 5.0,
                bool _useShoulder = 1,
                bool _useFoot = 1,
                bool _useBack = 0);

        enum LegPos {L0, L1, L2, R0, R1, R2, LEG_POS_MAX};
        enum LegPosUsage {LEG, WHEEL, UNUSED};
        enum LegJointType {
            // thoroca-coxal joint for forward (+) and backward (-) movements
            TC,
            // coxa-trochanteral joint for elevation (+) and depression (-) of
            // the leg
            CTR,
            // femur-tibia joints for extension (+) and flexion (-) of the
            // tibia
            FTI,
            // maximum value, used for iteration
            LEG_JOINT_TYPE_MAX
            };
        typedef AmosIIMotorNames MotorName;
        typedef AmosIISensorNames SensorName;

        /**
         * constructor of VierBeiner robot
         * @param odeHandle data structure for accessing ODE
         * @param osgHandle ata structure for accessing OSG
         * @param conf configuration object
         */
        AmosII( const OdeHandle& odeHandle,
                const OsgHandle& osgHandle,
                const AmosIIConf& conf,
                const std::string& name);

        virtual ~AmosII();

        /**
         * updates the OSG nodes of the vehicle
         */
        virtual void update();


        /** sets the pose of the vehicle
        @param pose desired pose matrix
        */
        virtual void place(const osg::Matrix& pose);

        /** returns actual sensorvalues
        @param sensors sensors scaled to [-1,1]
        @param sensornumbAmosII::getDefaultConf()er length of the sensor array
        @return number of actually written sensors
        */
        virtual int getSensors(sensor* sensors, int sensornumber);

        /** sets actual motorcommands
        @param motors motors scaled to [-1,1]
        @param motornumber length of the motor array
        */
        virtual void setMotors(const motor* motors, int motornumber);

        /** returns number of sensors
         */
        virtual int getSensorNumber();

        /** returns number of motors
         */
        virtual int getMotorNumber();
        /** checks for internal collisions and treats them.
         *  In case of a treatment return true (collision will be ignored by other objects
         *  and the default routine)  else false (collision is passed to other objects and
         *  (if not treated) to the default routine).
         */
        virtual bool collisionCallback(void *data, dGeomID o1, dGeomID o2);

        /** this function is called in each timestep. It should perform robot-internal checks,
        like space-internal collision detection, sensor resets/update etc.
        @param globalData structure that contains global data from the simulation environment
        */
        virtual void doInternalStuff(GlobalData& globalData);


        // virtual void AmosII::updateLegTouch(int);

        virtual double getMassOfRobot();

        void setLegPosUsage(LegPos leg, LegPosUsage usage);

        // Configurable Interface
        virtual bool setParam(const paramkey& key, paramval val);

        /** the main object of the robot, which is used for position and speed tracking */
        virtual Primitive* getMainPrimitive() const;

        /**
         * returns the MotorName enum value for the given joint at the given
         * leg. If the value for leg or joint are not valid AMOSII_MOTOR_MAX
         * is returned.
         *
         * @param leg leg position
         * @param joint leg joint type
         * @return the motor name value or AMOSII_MOTOR_MAX if parameters are
         *         invalid
         */
        static MotorName getMotorName(LegPos leg, LegJointType joint);

        /**
         * Returns the joint type of the given motor. If the given motor name
         * is not associated with a leg joint JOINT_TYPE_MAX is returend and a
         * warning is given out.
         *
         * @param MotorName name of the motor
         * @return joint type controlled by this motor or JOINT_TYPE_MAX if
         *         MotorName is invalid
         */
        static LegJointType getLegJointType(MotorName);

        /**
         * Returns the leg of the given motor. If the given motor name is not
         * associated wit a leg LEG_POS_MAX is returned and a warning is given
         * out.
         *
         * @param MotorName name of the motor
         * @return the leg on which this motor operates or LEG_POS_MAX if
         *         MotorName is invalid
         */
        static LegPos getMotorLegPos(MotorName);

    protected:

        struct Leg {
            Leg();
            HingeJoint   * tcJoint;
            HingeJoint   * ctrJoint;
            HingeJoint   * ftiJoint;
            SliderJoint  * footJoint;
            OneAxisServo * tcServo;
            OneAxisServo * ctrServo;
            OneAxisServo * ftiServo;
            Spring       * footSpring;
            Primitive    * shoulder;
            Primitive    * coxa;
            Primitive    * second;
            Primitive    * tibia;
            Primitive    * foot;
        };

        /**
         * creates vehicle at desired pose
         *
         * @param pose 4x4 pose matrix
         */
        virtual void create(const osg::Matrix& pose);

        /**
         * destroys vehicle and space
         */
        virtual void destroy();

        /**
         * Assign a human readable name to a motor. This name is used for the
         * associated inspectable value as used e.g. in guilogger.
         *
         * @param motorNo index of the motor (for standard motors defined by
         *        the MotorName enum)
         * @param name human readable name for the motor
         */
        void nameMotor(const int motorNo, const char* name);

         /**
         * Assign a human readable name to a sensor. This name is used for the
         * associated inspectable value as used e.g. in guilogger.
         *
         * @param motorNo index of the motor (for standard motors defined by
         *        the SensorName enum)
         * @param name human readable name for the sensor
         */
        void nameSensor(const int sensorNo, const char* name);

    private:

        /** typedefs */
        typedef std::map<LegPos,    HingeJoint*>   HingeJointMap;
        typedef std::map<LegPos,    Leg>           LegMap;
        typedef std::map<MotorName, OneAxisServo*> MotorMap;
        typedef std::map<LegPos,    LegPosUsage>   LegPosUsageMap;
        typedef std::map<LegPos,    IRSensor*>     LegIRSensorMap;
        typedef std::vector<Primitive*>            PrimitiveList;
        typedef std::vector<Joint*>                JointList;
        typedef std::vector<OneAxisServo*>         ServoList;

        AmosIIConf conf;
        bool created;      // true if robot was created

        /** a collection of ir sensors **/
        RaySensorBank irSensorBank;

        /** speed sensor */
        /** @todo implement speed sensor */
        SpeedSensor * speedsensor;

        /**
         * statistics
         * theses values are updated in every timestep and have to be updated
         * to make them available to the lpzrobots::Inspectable interface
         */
        /** position of the robot */
        Position position;

        /**
         * defines for everey leg position the way it is used (e.g  place
         * a leg or a wheel at this position)
         */
        LegPosUsageMap legPosUsage;

        /**
         * used for detection of leg contacts
         */
        LegMap mylegContactArray;

        // this map knows which IR sensor to find at which leg
        LegIRSensorMap irLegSensors;

        // holds the two front ultrasonic sensors
        IRSensor * usSensorFrontLeft;
        IRSensor * usSensorFrontRight;

        // body in case of no hinge joint being used
        Primitive *trunk;

        // front part of body (when hinge joint is used)
        Primitive *front;

        // back part of body (when hinge joint is used)
        Primitive *center;

        // information on all legs
        LegMap legs;

        // back bone joint
        OneAxisServo * backboneServo;

        // all the objects
        PrimitiveList objects;

        // all the joints
        JointList joints;

        // passive servos without a Motorname
        ServoList passiveServos;

        // contains all active servos
        MotorMap servos;
        std::map<LegPos, bool> legContact;
    };
}

#endif
