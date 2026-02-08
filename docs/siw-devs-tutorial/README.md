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
2. Wait for the environment to build, to inlcude activating the extensions.  
3. The UI will ask you for a build configuration, select debug.
4. Then cmake will build the project.
5. The Run and Debug tool (select in left margin) will have both a Run and Debug configuration to select.
6. Run the model

* adevs_irp fails because Repailer.cpp still needs and implementataion.

---

## Quick Start (Local Machine)
```
git clone https://github.com/simlytics-cloud/irp-adevs
cd irp-adevs
git fetch
git switch -c branch-name origin/siw-devs-tutorial-starter
```
* Build with cmake
* Run adevs_irp executable
* adevs_irp fails because Repailer.cpp still needs and implementataion.



## Retailer Model Tutorial — Scheduled DEVS Implementation Guide

This tutorial page explains how the **Retailer** model is implemented for the Inventory Routing Problem using the adevs. It provides the architectural background needed to complete the `Retailer.cpp` exercise class.

The Retailer model is built using adevs [adevs](https://web.ornl.gov/~nutarojj/adevs/docs/index.html). Much of the simulation timing and event mechanics are already implemented in the framework. Your job is to implement the domain behavior, not the simulation engine mechanics.

Before getting into the tutorial, take a few minutes to browse the adevs documentation page
for [Atomic Models](https://web.ornl.gov/~nutarojj/adevs/docs/manual/node5.html).  This shows an
implementation of a single server queueing Clerk model.  Additional information can be found
in the [adevs API documentationi](https://web.ornl.gov/~nutarojj/adevs/docs/api/hierarchy.html).

The [Retailer.h](../../src/irpmodel/Retailer.h) header file has the structure of the Retailer.
It is a template class where input is of type [IrpEvent](../../src/irp-iso/irpdomain/IrpEvent.h).
This is an abstract class where the actual input [Delivery](../../src/irp-iso/irpdomain/Delivery.h) and
output [InventoryCost](../../src/irp-iso/irpdomain/InventoryCost.h) are subclasses.  Ports are defined using the `std::string` type, and time is `long` integers represenint the minutes since the start of Day 1 at 00:00.  Also notice the definition of retailer events:
```
  private:
    enum class RetailerEventType {OPEN, CLOSE};
    struct RetailerEvent {
      RetailerEventType eventType;
      long time;
    };
```
We will use these event types in our implementation.  The `const` attributes do not change over
the life of a retailer, but the `currentInventory`, `nextEvent`, and `currentTime` will.


### What You Must Implement

For the Retailer tutorial exercise, you must complete the domain logic for a [Retailer.cpp](../../src/irpmodel/Retailer.cpp) implementation.

Implement handling of CLOSE and OPEN events in the internal state transition function

```
void Retailer::delta_int()
```

Implement handling of deliveries at the `receiveDelivery` port in the external state transition function.

```
void Retailer::delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
```

Implement the confluent state transition function.

```
void Retailer::delta_ext(long e, const adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& xb)
```

Implement the output function to sent InventoryCost to the dailyInventoryCost ouput port.

```
void Retailer::output_func(adevs::Bag<adevs::PortValue<IrpEvent*, std::string>>& yb) 
```

Implement the time advance function.
```
long Retailer::ta()
```

To test your implementation, you will run the adevs_irp test listed in the [main.cpp](../../src/irpmodeltest/main.cpp).  It builds a DEVS experimental frame where the [RetailerTestGenerator](../../src/irpmodeltest/ReceiverTestGenerator.h) generates two deliveries at noon each day to the retailer.  The retailer processes the deliveries and executes transition over two days, passing its output to the [RetailerTestAcceptor](../../src/irpmodeltest/RetailerTestAcceptor.h), which validates the correct behavior.

If your implementation is correct, you should get a passed test.  Use the debugger judiciously to test your partial implementation.

If you get stuck, the solution is on the siw-devs-tutorial-solution branch.

Happy Coding!


