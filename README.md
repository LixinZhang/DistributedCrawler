DistributedCrawler
==================

This is a Distributed Web Crawler Project using C++ on Linux platform . 
1. This project introduces Consistent Hash algorithm, which is used to solve the strategy of URL partition, hot-spot problem and load balancing between web crawler nodes and ensure that the distributed crawler has good scalability, balancing, fault tolerance.  
2. In order to meet the politeness and priority needs of the web crawler, this project designs and implements a URL queue based on Mercator model.  
3. The solutions to large-scale URLs deduplication,DNS resolution,page crawling and parsing and some other key problems are given.  
4. This project designs and implements a thread pool model for efficient and  multi-threaded page collection.  
5. A scheme for downloaded page storage is given, which creats indexd files and data files to manage and store the downloaded data.
