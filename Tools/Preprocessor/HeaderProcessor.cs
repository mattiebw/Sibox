using System.Text;

namespace Preprocessor;

public enum HeaderProcessorState
{
    LookingForClass,
    FoundPossibleClass,
    InClassInheritance,
    InEntityPrescope,
    InEntity,
    InIrrelevantScope,
    InIrrelevantParentheses,
    InProperty,
    InFunc,
    InRPC,
    SingleLineComment,
    MultiLineComment,
}

public class HeaderProcessor(string content)
{
    /// <summary>
    /// The horrible block of code that processes the header file.
    /// We've got a basic parser for C++ headers. It looks specifically for any class that inherits from Entity.
    /// If it does, it looks through it for any PROP, FUNC, or RPC macros, and processes them.
    /// If we find any, we package up the info into their appropriate classes and return them as an <see cref="Entity"/> class.
    /// </summary>
    /// <returns>A list of all entities found in the header. Returns an empty list if there's none.</returns>
    public List<Entity> Process()
    {
        var entities = new List<Entity>();
        StringBuilder buffer = new();
        Stack<HeaderProcessorState>
            stateStack = new(); // Each time we enter a new scope, we push a new state to the stack.
        Stack<StringBuilder>
            paramStack =
                new(); // Each time we enter new parentheses that we care about, we push a new buffer to the stack.
        string previousToken = "";
        string className = "";
        Entity? currentEntity = null;
        stateStack.Push(HeaderProcessorState.LookingForClass);

        int i = -1;
        foreach (char c in content)
        {
            i++;

            // If we are in a parameter we care about, we need to append the character to the buffers.
            if (stateStack.Contains(HeaderProcessorState.InRPC) || stateStack.Contains(HeaderProcessorState.InFunc) ||
                stateStack.Contains(HeaderProcessorState.InProperty))
            {
                foreach (var paramBuffer in paramStack) paramBuffer.Append(c);
            }

            if (stateStack.Peek() == HeaderProcessorState.MultiLineComment)
            {
                buffer.Append(c);
                if (buffer is [.., '*', '/'])
                {
                    stateStack.Pop();
                    buffer.Clear();
                }

                continue;
            }
            
            if (stateStack.Peek() == HeaderProcessorState.SingleLineComment)
            {
                if (c == '\n')
                {
                    stateStack.Pop();
                    buffer.Clear();
                }
            }
            else
                switch (c)
                {
                    case ' ' or '\n':
                    {
                        string spacedWord = buffer.ToString();
                        buffer.Clear();

                        switch (stateStack.Peek())
                        {
                            case HeaderProcessorState.LookingForClass:
                                if (spacedWord == "class")
                                    stateStack.Push(HeaderProcessorState.FoundPossibleClass);
                                break;
                            case HeaderProcessorState.InClassInheritance:
                                if (spacedWord == "Entity")
                                {
                                    stateStack.Pop();
                                    stateStack.Push(HeaderProcessorState.InEntityPrescope);
                                }

                                break;
                        }

                        previousToken = spacedWord;

                        break;
                    }
                    case '(':
                        string parameteredWord = buffer.ToString();
                        buffer.Clear();

                        bool Check(string word)
                        {
                            if (currentEntity is null)
                                return false;
                            
                            switch (word.Trim())
                            {
                                case "PROP":
                                    stateStack.Push(HeaderProcessorState.InProperty);
                                    paramStack.Push(new StringBuilder());
                                    return true;
                                case "FUNC":
                                    stateStack.Push(HeaderProcessorState.InFunc);
                                    paramStack.Push(new StringBuilder());
                                    return true;
                                case "RPC":
                                    stateStack.Push(HeaderProcessorState.InRPC);
                                    paramStack.Push(new StringBuilder());
                                    return true;
                                default:
                                    return false;
                            }
                        }

                        if (!Check(parameteredWord))
                        {
                            if (!Check(previousToken))
                                stateStack.Push(HeaderProcessorState.InIrrelevantParentheses);
                        }

                        previousToken = parameteredWord;

                        break;
                    case ')':
                        if (stateStack.Peek() == HeaderProcessorState.InIrrelevantParentheses)
                        {
                            stateStack.Pop();
                            break;
                        }

                        var paramContent = paramStack.Pop().ToString().Trim();
                        paramContent = paramContent.Remove(paramContent.Length - 1);
                        
                        switch (stateStack.Peek())
                        {
                            case HeaderProcessorState.InProperty:
                                stateStack.Pop();
                                var property = new Property(paramContent);
                                currentEntity!.Properties.Add(property);
                                break;
                            case HeaderProcessorState.InFunc:
                                stateStack.Pop();
                                var func = new Func(paramContent);
                                currentEntity!.Functions.Add(func);
                                break;
                            case HeaderProcessorState.InRPC:
                                stateStack.Pop();
                                var rpc = new RPC(paramContent);
                                currentEntity!.RPCs.Add(rpc);
                                break;
                        }

                        break;
                    case '{':
                        switch (stateStack.Peek())
                        {
                            case HeaderProcessorState.FoundPossibleClass:
                                if (buffer.Length != 0)
                                {
                                    className = buffer.ToString();
                                    buffer.Clear();
                                }
                                else
                                    className = previousToken;

                                if (className == "Entity")
                                {
                                    // We're in the Entity class, so we need to preprocess it anyway.
                                    stateStack.Pop();
                                    stateStack.Push(HeaderProcessorState.InEntity);
                                    currentEntity = new Entity(className);
                                }
                                else
                                    stateStack.Push(HeaderProcessorState.InIrrelevantScope);

                                break;
                            case HeaderProcessorState.InEntityPrescope:
                                stateStack.Pop();
                                stateStack.Push(HeaderProcessorState.InEntity);
                                currentEntity = new Entity(className);
                                break;
                            default:
                                stateStack.Push(HeaderProcessorState.InIrrelevantScope);
                                break;
                        }

                        break;
                    case '}':
                        switch (stateStack.Peek())
                        {
                            case HeaderProcessorState.InEntity:
                                entities.Add(currentEntity!);
                                currentEntity = null;
                                className = "";
                                stateStack.Pop();
                                break;
                            case HeaderProcessorState.InIrrelevantScope:
                                stateStack.Pop();
                                break;
                            default:
                                Console.WriteLine("Unaccounted scope closing!");
                                break;
                        }

                        break;
                    case ':':
                        switch (stateStack.Peek())
                        {
                            case HeaderProcessorState.FoundPossibleClass:
                                if (buffer.Length != 0)
                                {
                                    className = buffer.ToString();
                                    buffer.Clear();
                                }
                                else
                                    className = previousToken;

                                stateStack.Pop();
                                stateStack.Push(HeaderProcessorState.InClassInheritance);
                                break;
                        }

                        break;
                    case ';':
                        switch (stateStack.Peek())
                        {
                            case HeaderProcessorState.FoundPossibleClass:
                                // It was a forward declaration, so we don't care about it.
                                stateStack.Pop();
                                break;
                            case HeaderProcessorState.InProperty:
                                break;
                            case HeaderProcessorState.InFunc:
                                break;
                            case HeaderProcessorState.InRPC:
                                break;
                        }

                        break;
                    case '/':
                        buffer.Append(c);
                        if (buffer is [.., '/', _])
                            stateStack.Push(HeaderProcessorState.SingleLineComment);
                        break;
                    case '*':
                        switch (stateStack.Peek())
                        {
                            case HeaderProcessorState.InProperty:
                                buffer.Append(c);
                                break;
                            case HeaderProcessorState.InFunc:
                                buffer.Append(c);
                                break;
                            case HeaderProcessorState.InRPC:
                                buffer.Append(c);
                                break;
                            default:
                                buffer.Append(c);
                                if (buffer is [.., '/', _])
                                    stateStack.Push(HeaderProcessorState.MultiLineComment);
                                break;
                        }

                        break;
                    default:
                        if (char.IsWhiteSpace(c))
                            break;
                        
                        if (stateStack.Peek() == HeaderProcessorState.SingleLineComment)
                            continue;

                        buffer.Append(c);
                        break;
                }
        }

        return entities;
    }
}
