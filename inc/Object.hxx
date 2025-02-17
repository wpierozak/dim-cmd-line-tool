class Object
{
public:
    enum class State{Error, StandBy, Active};

    State state()
    {
        return m_state;
    }

protected:
    void changeState(State state)
    {
        m_state = state;
    }

private:
    State m_state;
};