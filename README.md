# Loki11

Loki11是一个只由头文件组成的C++模板库, 是《Modern C++ Design》一书中描述的Loki库的C++11/14版.

《[Modern C++ Design](https://en.wikipedia.org/wiki/Modern_C%2B%2B_Design)》: 该书是由 [Andrei Alexandrescu](https://en.wikipedia.org/wiki/Andrei_Alexandrescu) 撰写, 于2001年出版. 
本书提出了泛型组件的概念, 这是一种可重用的设计模板, 可用来产生规范的C++代码. 泛型组件还提供了从设计到代码的无缝转换, 生成更能表达设计者最初意图的代码, 并支持设计模式的重用——只需改动少量的代码. 
本书是一部重要的著作, 全面地展示了泛型模式或模式模板在C++中创建可扩展设计的功能强大的新方法. 

## 什么是Loki/Loki11

**Loki** 是一个C++编写的模板程序库. 包括Small-Object Allocation, Generalization Funtors, Singleton, Smart Pointers, Object Factories, Abstract Factory, Visitor, Multimethods 等泛型组件. 
以templates-based, policies-based等手法, 将一些设计模式具体实现.

## 进度
### 第二章

| 内容             | 文件             | 描述                   |
| :--              | :--              | :--                    |
| 编译期Assertions | static_check.hpp | 编译期错误检测         |
| 长整数映射为类型 | TypeManip.hpp    | Int2Type               |
| 类型映射为类型   | TypeManip.hpp    | Type2Type              |
| 编译期类型选择   | TypeManip.hpp    | 编译期IF语句           |
| 类型可转换性     | TypeManip.hpp    | 类型间的可转换关系     |
| 类型继承性       | TypeManip.hpp    | 类型间的继承关系       |
| type_info外覆类  | LokiTypeInfo.hpp | 对std::type_info的包装 |
| NullType         | NullType.hpp     | 空类型                 |
| EmptyType        | EmptyType.hpp    | 空类型                 |
| Type Traits      | TypeTraits.hpp   | 类型信息萃取                 |

### 第三章 ###

| 内容          | 文件                   | 描述                           |
| :--           | :--                    | :--                            |
| 类型列表      | Typelist.hpp           | 对一系列类型组成的列表的操作   |
| 自动产生class | HierarchyGenerator.hpp | 自动产生散乱的或线性的继承体系 |

### 第四章 ###

| 内容 | 文件 | 描述 |
| :--  | :--  | :--  |
| 小型对象分配技术 | SmallObj.hpp | 一个高效的小型对象分配器 |

### 第五章 ###

| 内容 | 文件 | 描述 |
| :--  | :--  | :--  |
| 泛化仿函数 | Functor.hpp | 一种简单的函数封装对象 |

### 第六章 ###

| 内容 | 文件 | 描述 |
| :--  | :--  | :--  |
| 单例模式实作技术 | Singleton.hpp | 一个支持多种单例模式的模板 |

### 附录 ###

| 内容 | 文件 | 描述 |
| :--  | :--  | :--  |
| 多线程 | Threads.hpp | 一个超迷你的多线程程序库, 为Loki定制 |
