namespace ir {
	namespace detail {
		namespace states {
			template <typename T>
			concept NoRttiReserved =
				!requires(T t) { t.meta_name(); } &&
				!requires(T t) { t.meta_load(); } &&
				!requires(T t, ir::StateMachine* sm) { t.meta_setStateMachine(sm); } &&
				!requires(T t) { t.meta_exit(); };
		}

		class State {
		private:
			class Concept {
			public:
				virtual void onInitialize() = 0;
				virtual void onReceiveEvent(const sf::Event& event) = 0;
				virtual void onUpdate() = 0;
				virtual void onRender() = 0;
				virtual void onEnd() = 0;

				virtual void meta_setStateMachine(ir::StateMachine* sm) = 0;
				virtual void meta_exit() = 0;
				virtual void meta_setContext(ApplicationContext* context) = 0;
			};

			template <typename T>
			class Model : public Concept {
			private:
				T value;

			public:
				Model(T val) : value(std::move(val)) {}

				void onInitialize() override {
					value.onInitialize();
				}

				void onReceiveEvent(const sf::Event& event) override {
					value.onReceiveEvent(event);
				}

				void onUpdate() override {
					value.onUpdate();
				}

				void onRender() override {
					value.onRender();
				}

				void onEnd() override {
					value.onEnd();
				}

				void meta_setStateMachine(ir::StateMachine* stateMachine) override {
					value.meta_setStateMachine(stateMachine);
				}

				void meta_exit() override {
					value.meta_exit();
				}

				void meta_setContext(ApplicationContext* context) {
					value.meta_setContext(context);
				}
			};
			
			std::unique_ptr<Concept> self_;

		public:
			template <typename T>
			State(T value) : self_(std::make_unique<Model<T>>(std::move(value))) {}

			void onInitialize() {
				self_->onInitialize();
			}

			void onReceiveEvent(const sf::Event& event) {
				self_->onReceiveEvent(event);
			}

			void onUpdate() {
				self_->onUpdate();
			}

			void onRender() {
				self_->onRender();
			}

			void onEnd() {
				self_->onEnd();
			}

			void meta_setStateMachine(ir::StateMachine* stateMachine) {
				self_->meta_setStateMachine(stateMachine);
			}

			void meta_setContext(ApplicationContext* context) {
				self_->meta_setContext(context);
			}
		};
	}
}