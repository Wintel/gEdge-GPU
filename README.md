# gEdge - Container-Based Heterogeneous Computing Framework for Edge-Cloud Collaboration

**Table of Contents**
- [Introduction](#introduction)
- [Key Features](#key-features)
- [Resource Isolation](#resource-isolation)
- [Container Optimization](#container-optimization)
- [Performance](#performance)
- [Conclusion](#conclusion)

## Introduction

gEdge is an innovative heterogeneous computing framework designed for edge-cloud collaborative computing scenarios. It introduces container technology to seamlessly offload GPU container tasks from edge nodes to cloud servers, harnessing the powerful GPU resources in the cloud to accelerate computations. This addresses the issue of insufficient edge computing power due to energy constraints.

## Key Features

### 1. Transparent Offloading

gEdge enables the transparent offloading of GPU container tasks between edge devices and cloud servers, ensuring load balancing and maximizing the utilization of edge device computing capabilities. This allows for efficient task distribution, reducing the burden on cloud servers.

### 2. Application-Aware Resource Isolation

In multi-tenant environments, gEdge employs application-sensitive resource isolation strategies tailored to different types of GPU workloads. This provides excellent resource isolation for containers on edge nodes, increasing the efficiency of cloud GPU resource utilization and ensuring fair allocation among tasks.

### 3. Edge-Friendly Container Optimization

To address resource limitations on edge nodes, gEdge offers container optimization solutions that reduce container image size and enhance container startup speed. This results in improved container density, allowing more containers to run concurrently on edge devices.

## Resource Isolation

gEdge prioritizes resource isolation to ensure efficient and secure operation:

- **GPU Resource Isolation:** It employs resource isolation mechanisms that guarantee GPU resources are fairly and securely allocated among containers, preventing resource contention.

- **Security:** gEdge ensures task security by isolating containers, preventing unauthorized access or interference between different tasks.

## Container Optimization

gEdge optimizes containers for efficient edge computing:

- **Container Image Size Reduction:** By minimizing container image sizes, gEdge decreases the storage and bandwidth requirements, making it more suitable for edge environments with limited resources.

- **Container Startup Speed:** Faster container startup times result in quicker task execution, improving overall system responsiveness and efficiency.

## Performance

gEdge's performance has been rigorously tested and validated:

- **Significant Speedup:** In experiments, gEdge achieved an average performance improvement of 213% when considering offloading latency. This demonstrates its effectiveness in harnessing cloud GPU resources to accelerate computations.

- **Effective Multi-Tenancy:** In multi-tenant scenarios, gEdge successfully provides excellent resource isolation, ensuring fair and efficient resource sharing among concurrently running containers.

## Conclusion

In summary, gEdge revolutionizes edge-cloud collaboration by transparently offloading tasks, achieving load balancing, and efficiently utilizing edge device computing power. It also offers application-aware resource isolation for task fairness and security. Lastly, its edge-friendly container optimization facilitates easy deployment and scalability across various edge devices and cloud environments.

gEdge is a powerful framework that empowers developers to harness the potential of edge computing while seamlessly integrating with cloud resources, opening up new possibilities for a wide range of applications.

For more details and usage instructions, please refer to the project documentation.

mail:yunwang94 AT sjtu.edu.cn
