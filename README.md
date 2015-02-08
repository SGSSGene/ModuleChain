# ModuleChain

This library makes it very simple expressing "require" and "provide" relationships.

## Example
### How to register modules:
```c
	class Module1 {
		moduleChain::Provide<std::string> message;
	public:
		void execute() {
			*message = "Hello World";
		}
	};
	REGISTER_MODULE(Module1)

	class Module2 {
		moduleChain::Require<std::string> message;
	public:
		void execute() {
			std::cout<<*message<<std::endl;
		}
	};
	REGISTER_MODULE(Module2)

	class Module3 {
		moduleChain::Require<std::string> message;
	public:
		void execute() {
			std::cerr<<*message<<std::endl;
		}
	};
	REGISTER_MODULE(Module3)

```

### How to run modules in one layer
```c
int main() {
	int threadCount = 2; // working with to threads
	moduleChain::Chain chain("testChain", {"Module1", "Module2", "Module3"}, threadCount);
	chain.run();
	return 0;
}
```
