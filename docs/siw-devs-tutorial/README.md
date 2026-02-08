# DEVS Tutorial — Starter: Inventory Routing Problem

Welcome to the **DEVS Tutorial starter project**. In this exercise you will implement a small but complete workflow using [adevs](https://web.ornl.gov/~nutarojj/adevs/docs/index.html) (a Discrete EVent system Simulator) inside a working sample [Inventory Routing Problem](https://github.com/simlytics-cloud/irp-adevs) application.  By the end of this tutorial you will be able to:

* Use [adevs](https://web.ornl.gov/~nutarojj/adevs/docs/index.html) to build a simple DEVS model.
* Run and validate results with the provided tests

---

## Prerequisites

This tutorial runs on [GitHub Codespaces](https://github.com/features/codespaces), so you can do everything there is you have a GitHub account.

* (Optional) Local toolchain Prerequisites:
    * Git
    * C++ Compiler
      * GCC ≥ 7
      *  Clang ≥ 6
      *  MSVC 19.14+ (Visual Studio 2017 15.7) or newer
    * CMmake

---

## Quick Start (Recommended: Codespaces)

1. Click **Code → Open in Codespaces**
2. Wait for the environment to build.  It takes a little while to download all Maven dependencies and compile DEVS Streaming Frarmework.
3. Open a terminal
4. Make sure you are on the siw-devs-tutorial-starter branch.  If not run
```
git fetch
git switch -c branch-name origin/siw-devs-tutorial-starter
```

5. Run:

```
mvn clean install
```

You should see:

* project builds
* tests run
* The RetailerImplTest fails because you will have to implement it.

---

## Quick Start (Local Machine)

```
git clone https://github.com/simlytics-cloud/devs-streaming.git
cd devs-streaming
mvn -DskipTests install
```
Wait a bit for DEVS Streaming Framework to download all dependecies and build.  After build succes:
```
cd ..
git clone https://github.com/simlytics-cloud/irp-java.git
cd irp-java
git fetch
git switch -c branch-name origin/siw-devs-tutorial-starter
mvn install
```
* Project builds
* Tests run
  *The RetailerImplTest fails because you will have to implement it.



## Retailer Model Tutorial — Scheduled DEVS Implementation Guide

This tutorial page explains how the **Retailer** model is implemented for the Inventory Routing Problem using the DEVS Streaming Framework model pattern. It provides the architectural background needed to complete the `RetailerImpl` exercise class.

The Retailer model is built using the DEVS Streaming Framework’s [ScheduledDevsModel](https://github.com/simlytics-cloud/devs-streaming/blob/main/src/main/java/devs/ScheduledDevsModel.java). Much of the simulation timing and event mechanics are already implemented in the framework. Your job is to implement the domain behavior, not the simulation engine mechanics.

The inheritance chain is:

```
ScheduledDevsModel (DEVS Streaming Framework)
    ↑
Facility (generated)
    ↑
Retailer (generated)
    ↑
RetailerImpl (your implementation)
```

## ScheduledDevsModel Execution Model

A typical DEVS atomic model reasons about time using a time advance functions and state transitions.
While this logic is mathematically necessary and succinct for DEVS formulation, it is different
than how simulation developers typically think about models with a clock and event schedule.  A
[ScheduledDevsModel](https://github.com/simlytics-cloud/devs-streaming/blob/main/src/main/java/devs/ScheduledDevsModel.java)
provides a DEVS compliant atomic model with an internal schedule.  Its internal state has a schedule, a time-ordered
TreeMap with a list of events and outputs at each time.

Because of this, the framework already implements several DEVS functions for you.

### Time Advance Function

The time advance function is already implemented.

Behavior:

* Returns the interval between:

    * current simulation time
    * the first scheduled item in the schedule

You do not implement time advance yourself.


### Output Function

The output function is already implemented.

Behavior:

* Returns a **bag of PortValues**
* Includes all outputs scheduled for the current simulation time

Outputs are pulled directly from the schedule.


### Internal State Transition

The internal transition function is also implemented by the framework.

It automatically:

* Advances current simulation time
* Removes published outputs from the schedule
* Retrieves all scheduled internal events at the current time
* Passes those events to your handler method

You do NOT override the internal transition directly.

Instead, you implement:

```
public void handleScheduledEvents(List<Object> events)
```

This is where your model reacts to scheduled internal events.


### Working with the Schedule

The schedule supports both internal events and scheduled outputs.
A common pattern is to define **inner classes** to represent internal events.


Example — schedule a store opening event 6 hours after simulation start:

```java
modelState.getSchedule().scheduleInternalEvent(
    LongSimTime.create(60 * 6),
    new OpenEvent()
);
```

Typical usage:

* define small inner event classes
* schedule them at future times
* handle them inside `handleScheduledEvents`


You can schedule output port values directly onto the schedule.

Example — publish daily inventory cost on the Retailers dailyInventoryCost port:

```java
modelState.getSchedule().scheduleOutput(
    currentTime, // Time of the scheduled output
    Retailer.dailyInventoryCost, // Port on which to place the output
    immutableInventoryCost // The output data structure consitent with the port type
);
```

These outputs will automatically be emitted by the framework output function at the scheduled time.

---

## Role of the Generated Classes

Much of the DEVS struture for this project was automatically generated into the [generated](../../generated/src/main/java/iso/example/irpsystem/) package.  You should not edit this code.  However the following classes help you understand the Retailer's implementation.

* [Facility](../../generated/src/main/java/iso/example/irpsystem/irpmodel/InventoryRouting/Facility.java) class is a lightweight shared parent class.  It holds shared state values and properties common to [Retailers](../../generated/src/main/java/iso/example/irpsystem/irpmodel/InventoryRouting/Retailer.java) and [Manufacturers](../../generated/src/main/java/iso/example/irpsystem/irpmodel/InventoryRouting/Manufacturer.java) class

* [Retailer](../../generated/src/main/java/iso/example/irpsystem/irpmodel/InventoryRouting/Retailer.java) is an abstract class that contains the internal state and structure of the retailers.  Static data is on its `properties` field, and dynamic data is in its `modelState`.  Its ports are defined as `public static final ImmutablePort<>` with a port name and data type.

The Retailer model includes an external transition that is already partially implemented.

* Advances current time correctly
* Iterates over incoming PortValues
* Dispatches by port name and value type

Incoming deliveries are forwarded to:

```
handleReceiveDelivery(
    ImmutableDelivery immutableDelivery,
    LongSimTime elapsedTime
)
```

You are responsible for completing this handler.

---

### What You Must Implement

For the Retailer tutorial exercise, you must complete the domain logic for:


Implement handling of deliveries at the `receiveDelivery` port

```
handleReceiveDelivery(...)
```

Implement handling of internal events, `OpenEvent` and `CloseEvent`

```
handleScheduledEvents(List<Object> events)
```


Implement the confluent transition behavior where internal and external events occur at the same simulation time.  A typical strategy is to decide whether you want to handle the input before or after
the internal event.  Based on that decisioin, call `externalStateTransitionFunction(LongSimTime.create(0), inputs);`
and `internalStateTransitionFunction();` in the corresponding order.


### Time Representation

Simulation time uses `LongSimTime`.  This represents minutes since simulation start. To get the minutes:

```java
long minutes = currentTime.getT();
```

To create a new value:

```java
LongSimTime scheduledEventTime = LongSimTime.create(60 * 14);
```

Use this for:

* computing delays
* scheduling future events
* calculating costs or penalties

---

## Implementation Strategy


1. Read generated [Retailer](../../generated/src/main/java/iso/example/irpsystem/irpmodel/InventoryRouting/Retailer.java) class
2. Review [ScheduledDevsModel](https://github.com/simlytics-cloud/devs-streaming/blob/main/src/main/java/devs/ScheduledDevsModel.java) schedule and understand its handler pattern.
3. Study [ManufacturerImpl](../../impl/src/main/java/iso/example/irpsystem/irpmodel/impl/ManufacturerImpl.java) as a complete example.
4. Open [RetailerImpl](../../impl/src/main/java/iso/example/irpsystem/irpmodel/impl/RetailerImpl.java).  Look for the TODO tags
4. Implement delivery handler
5. Implement scheduled event handler
6. Implement confluent logic
7. Run [RetailerImplTest](../../impl/src/test/java/iso/example/irpsystem/irpmodel/impl/RetailerImplTest.java)

If your implementation is correct, you should get a passed test.

If you get stuck, the solution is on the siw-devs-tutorial-solution branch.

Happy Coding!


