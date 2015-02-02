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
```

### How to run modules in one layer
```c
int main() {
	moduleChain::Layer layer("testLayer", {"Module1", "Module2"});
	layer.run();
	return 0;
}
```
