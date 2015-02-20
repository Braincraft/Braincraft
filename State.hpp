#ifndef STATE_HPP
#define STATE_HPP


#include "sstring.h"
#include "assume.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

//names ne sert à rien, non ?

/** Implements a generic minimal iterative structure.
 * Such a value corresponds to:
 * - scalar values: boolean, number, or string;
 * - value list, indexed from <tt>0</tt>, written <tt>[value, ...]</tt>;
 * - parameter set, written <tt>{name = value, ...}</tt>.
 * Key properties of such data structure:
 * - They corresponds to <a href="http://json.org/">JSON</a> structures (i.e. correspond to a data object model).
 * - By construction (value copy when set), they can
 * neither be cyclic (i.e. and infinite tree with a leaf linked to an ancestor),
 * nor have common branches (i.e. two Struct instance sharing the same value in memory).
 * - When a named parameter is set,
 * the previous scalar value if any is erased, while
 * the previous ordered list of value is now considered as value indexed by number converted as name.
 */
class State
{
	//
	// -1- Scalar value implementation
	//
  private:
	std::string value;
  public:
  /** Returns this value as a string or the empty string if undefined.
   * @return Either
   * - The string representation of the value if the data is atomic (i.e. corresponds to a boolean, number, or string), or
   * - The strict <a href="http://json.org/">JSON</a> syntax representation of this data-structure.
   */
	operator  String() const
	{
		if((values.size() > 0) && (value == ""))
		{
			StateWriter writer;
			  writer.write(const_cast < State * >(this)->value, *this);
		}
		return value;
	}
	State(const char *value)
	{
		clear();
		this->value = value;
	}
	State(String value)
	{
		clear();
		this->value = value;
	}
  public:
  /** Returns true if the value is "true", false otherwise.
   * @return The boolean <tt>true</tt> value if the value is "true", "True" or "TRUE".
   */
	operator  bool()const
	{
		return value == "true" || value == "True" || value == "TRUE";
	}
	State(bool value)
	{
		clear();
		this->value = value ? "true" : "false";
	}
  public:
  /** Returns the data as a floating point number or NAN if undefined. */
	operator  double () const
	{
		return toDouble(value);
	}
	State(double value)
	{
		clear();
		char chars[256];
		sprintf(chars, "%.16g", value);
		this->value = chars;
	}
	bool isDouble(void) const
	{
		char* endptr = 0;
		strtod(value.c_str(), &endptr);
		
		if(*endptr != '\0' || endptr == value.c_str())
			return false;
		return true;
	}
	bool isBool(void) const
	{
		if(value.size() != 4 && value.size() != 5)
			return false;
		std::string tmp = value;
		for(int i = 0; i < tmp.size(); ++i)
			tmp[i] = tolower(tmp[i]);
		return tmp == "true" || tmp == "false";
	}
  private:
	// Converts a string a number
	static double toDouble(String name)
	{
		char *end;
		double r = strtod(name.c_str(), &end);
		return *end == '\0' ? r : NAN;
	}
  public:
  /** Returns the data as an int or 0x10000000 (INT_MIN) if undefined. */
	operator  int () const
	{
		return toInt(value);
	}
	State(int value)
	{
		clear();
		char chars[64];
		sprintf(chars, "%d", value);
		this->value = chars;
	}
	State(unsigned int value)
	{
		clear();
		char chars[64];
		sprintf(chars, "%u", value);
		this->value = chars;
	}
	static const int INT_NAN = INT_MIN;
  private:
	// Converts a string a integer
	static int toInt(String name)
	{
		char *end;
		int r = strtol(name.c_str(), &end, 10);
		return *end == '\0' ? r : INT_NAN;
	}
	// Converts an integer to a string
	static std::string toName(int index)
	{
		static char s[64];
		sprintf(s, "%d", index);
		return s;
	}
  public:
  /** Returns true if this corresponds to a scalar (boolean, numeric, string) value and false otherwise. */
	bool isAtomic()const
	{
		return values.size() == 0;
	}
  /** Returns true if this corresponds to an empty scalar value corresponding to the empty string and false otherwise. */
	bool isEmpty() const
	{
		return values.size() == 0 && value == "";
	}
	//
	// -2- Iterative value implementation
	//
  private:
	// Data internal storage with index comparator
	  std::vector < std::string > names;
	std::map < std::string, State * >values;
	// Maximal indexed value
	int length;
	// Clears the values
	void clear()
	{
		for(std::map < std::string, State * >::const_iterator i = values.begin(); i != values.end(); i++)
		{
			i->second->clear();
			delete i->second;
		}
		values.clear();
		names.clear();
		value = "";
		length = 0;
	}
	// Recursively copy the values
	void copy(State * dst, const State & src)
	{
		dst->clear();
		dst->value = src.value;
		dst->length = src.length;
		for(std::map < std::string, State * >::const_iterator i = src.values.begin(); i != src.values.end(); i++)
		{
			State *s = new State();
			copy(s, *i->second);
			dst->values[i->first] = s;
		}
		for(std::vector < std::string >::const_iterator i = src.names.begin(); i != src.names.end(); i++)
		{
			dst->names.push_back(*i);
		}
	}
	// Inserts a value entry
	void insert(String name, bool setting)
	{
		if(values[name] == NULL)
		{
			// Creates an empty value
			State *s = new State();
			values[name] = s;
			// Manages the names and length
			int l = toInt(name);
			if(l == INT_NAN)
			{
				names.push_back(name);
			}
			else if(l >= length)
			{
				length = l + 1;
			}
			// Resets the corresponding string value
			if(setting)
			{
				this->value = "";
			}
		}
	}
	// Removes a value entry
	void erase(String name)
	{
		if(values[name] != NULL)
		{
			// Removes the value
			values[name]->clear();
			delete values[name];
			values.erase(name);
			// Manages the names and length
			int l = toInt(name);
			if(l == INT_NAN)
			{
				names.erase(std::remove(names.begin(), names.end(), name), names.end());
			}
			else
			{
				length = 0;
				for(std::map < std::string, State * >::const_iterator i = values.begin(); i != values.end(); i++)
				{
					int l = toInt(i->first);
					if((l != INT_NAN) && (length < l + 1))
					{
						length = l + 1;
					}
				}
			}
			// Resets the corresponding string value
			this->value = "";
		}
	}
	// Cleans empty values and verifies the structure integrity
	void clean()
	{
		int count = 0;
		for(int i = 0; i < length; i++)
		{
			String n = toName(i);
			if(values.count(n) > 0)
			{
				State *s = values[n];
				assume(s != NULL, "illegal-state", "Spurious NULL value for index %d in «%s»\n", i, value.c_str());
				if(s->isEmpty())
				{
					erase(n);
					count++;
				}
			}
		}
		for(std::vector < std::string >::iterator i = names.begin(); i != names.end(); i++)
		{
			String n = *i;
			assume(values.count(n) == 1, "illegal-state", "Spurious count %d of named value %s in %s\n", values.count(n), n.c_str(), value.c_str());
			State *s = values[n];
			assume(s != NULL, "illegal-state", "Spurious NULL value for name %s in «%s»\n", n.c_str(), value.c_str());
			if(s->isEmpty())
			{
				erase(*i);
				count++;
			}
		}
	}
  public:
	// Constructs an empty data structure.
	State()
	{
		clear();
	}
	// Copies a data structure.
	State(const State & value)
	{
		copy(this, value);
	}
	// Deletes a data structure.
	~State()
	{
		clear();
	}

  /** Returns true the value is undefined or corresponds to the empty string and false otherwise.
   * @param name The name or the index of the value.
   */
	bool isEmpty(String name) const
	{
		return values.count(name) == 0 || const_cast < State * >(this)->values[name]->isEmpty();
	}
	bool isEmpty(int index)const
	{
		return isEmpty(toName(index));
	}
  /** Returns the reference to a named or indexed value.
   * - Also available though the <tt>struct[name]</tt> subscript operator.
   * @param name The name or the index of the value.
   * @return A reference to the value, creating this entry if not yet defined.
   */
	State & get(String name) const
	{
		State *s = const_cast < State * >(this); //WHY
		  s->insert(name, false);
		  return *s->values[name];
	}
	State & get(int index)const
	{
		return get(toName(index));
	}
	State & operator[] (String name)
	{
		return get(name);
	}
	State & operator[](const char *name)
	{
		return get(name);
	}
	State & operator[](int index)
	{
		return get(index);
	}

  /** Sets a named or indexed value.
   * - Also available though the <tt>struct[name] = value</tt> assignment/subscript operators.
   * @param name The name or the index of the value.
   * @param value The value to set.
   */
	State & set(String name, const State & value)
	{
		insert(name, true);
		copy(values[name], value);
		return *this;
	}
	State & set(int index, const State & value)
	{
		return set(toName(index), value);
	}
	State & operator =(const State & value)
	{
		copy(this, value);
		return *this;
	}
  /** Appends an indexed value. */
	State & add(const State & value)
	{
		return set(getLength(), value);
	}
  /** Erases a named or indexed value. */
	State & del(String name)
	{
		erase(name);
		return *this;
	}
	State & del(int index)
	{
		return del(toName(index));
	}
  /** Returns the maximal indexed value in the structure.
   * - It is used in a construct of the form:
   *  <div><tt>for(int i = 0; i < value.getLength(); i++) { State &value = value.get(i); ../.. }</tt>.</div>
   */
	int getLength() const
	{
		return length;
	}
  /** Returns the number of named value in the structure. */
	int getCount() const
	{
		return names.size();
	}
  /** Defines an iterator over the parameters.
   * - It is used in a construct of the form:
   *  <div><tt>for(State::Iterator i(values); i.next();) { String name = i.getName(); State &value = i.getValue(); ../.. }</tt>.</div>
   */
	class Iterator
	{
	  private:
		const std::vector < std::string > &names;
		const std::map < std::string, State * >&values;
		int index;
	  public:
	/** Constructs an iterator over the logical-structure fields.
     * @param value The logical-structure to iterate on.
     */
		  Iterator(const State & value):names(value.names), values(value.values)
		{
			index = -1;
		}
	/** Returns the current name if any, else the result is undefined. */
		String getName() const
		{
			return names[index];
		}
	/** Returns the current value if any, else the result is undefined. */
		State & getValue() const
		{
			return *values.find(getName())->second;
		}
	/** Returns the current name index, starting from 0 up to getCount()-1. */
		int getIndex() const
		{
			return index;
		}
	/** Returns true if there is another element, enumerating a new element at each call. */
		bool next()
		{
			index++;
			return index < (int)names.size();
		}
	};

  /** Returns true if this structure equals the comparing one.
   * - Also available via the <tt>==</tt> and <tt>!=</tt> operators.
   */
	bool equals(const State & value) const
	{
		if(isAtomic())
		{
			if(value.isAtomic())
			{
				return this->value == value.value;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if(value.isAtomic())
			{
				return false;
			}
			else
			{
				// Calculates the union of both names
				std::set < std::string > names;
				{
					for(std::map < std::string, State * >::const_iterator i = values.begin(); i != values.end(); i++)
					{
						names.insert(i->first);
					}
					for(std::map < std::string, State * >::const_iterator i = value.values.begin(); i != value.values.end(); i++)
					{
						names.insert(i->first);
					}
				}
				for(std::set < std::string >::const_iterator i = names.begin(); i != names.end(); i++)
				{
					if((values.count(*i) == 0) || (value.values.count(*i) == 0))
					{
						return false;
					}
					State *s1 = const_cast < State & >(*this).values[*i], *s2 = const_cast < State & >(value).values[*i];
					if(!s1->equals(*s2))
					{
						return false;
					}
				}
				return true;
			}
		}
	}
	bool operator ==(const State & value)const
	{
		return equals(value);
	}
	bool operator !=(const State & value)const
	{
		return !equals(value);
	}

	//
	// -3- String input/output interface
	//

  /** Saves the data structure as a json strong compact syntax string, in a file.
   * @param location The file path. If "stdout", print the data in the console.
   * @param format The output format:
   * - "raw" : 1D raw compact format with minimal space.
   * - "plain" : 2D human editable format with tabulation.
   * - "html" : HTML human readable format.
   */
	void save(String location, String format = "raw")
	{
		std::string string;
		{
			StateWriter writer;
			writer.write(string, *this, format);
		}
		if(location == "stdout")
		{
			printf(string.c_str());
		}
		else
		{
			FILE *fp = fopen(location.c_str(), "w");
			assume(fp != NULL, "IO-exception", "in State::save: %s not writable", location.c_str());
			fprintf(fp, "%s\n", string.c_str());
			fclose(fp);
		}
	}
  private:
	// Implements the string writing of a json structure
	class StateWriter
	{
	  public:
		// Writes a value in a string, in  compact-raw (mode = 0), 2D-plain-text (mode = 1), html-text (mode = 2).
		void write(std::string & string, const State & value, String format = "raw")
		{
			tab = 0, mode = format == "html" ? 2 : format == "plain" ? 1 : 0;
			string = stringHeader();
			write_value(string, value);
			string += stringTrailer();
		}
	  private:
		// Recursively writes a value
		void write_value(std::string & string, const State & value)
		{
			if(value.isAtomic())
			{
				write_word(string, value.value);
			}
			else if(value.getCount() == 0)
			{
				string += asBeginTag("[");
				for(int i = 0, l = value.getLength() - 1; i <= l; i++)
				{
					if(!value.isEmpty(i))
					{
						write_value(string, const_cast < State & >(value).get(i));
					}
					string += (i < l ? asNextTag(",") : asEndTag("]"));
				}
			}
			else
			{
				string += asBeginTag("{");
				for(State::Iterator i(const_cast < State & >(value)); i.next();)
				{
					if(!i.getValue().isEmpty())
					{
						write_word(string, i.getName(), true);
						string += asMeta(":", true);
						write_value(string, i.getValue());
						string += (i.getIndex() < value.getCount() - 1 || value.getLength() > 0 ? asNextTag(",") : "");
					}
				}
				for(int i = 0, l = value.getLength() - 1; i <= l; i++)
				{
					if(!value.isEmpty(i))
					{
						write_word(string, toName(i), true);
						string += asMeta(":", true);
						write_value(string, const_cast < State & >(value).get(i));
						string += (i < l ? asNextTag(",") : "");
					}
				}
				string += asEndTag("}");
			}
		}
		void write_word(std::string & string, String value, bool name = false)
		{
			string += asBeginValue("\"", name);
			for(unsigned int i = 0; i < value.length(); i++)
			{
				switch (value[i])
				{
				case '"':
				case '\\':
					string += "\\";
					string += value[i];
					break;
				case '\n':
					string += "\\n";
					break;
				case '\b':
					string += "\\b";
					break;
				case '\r':
					string += "\\r";
					break;
				case '\t':
					string += "\\t";
					break;
				case '\f':
					string += "\\f";
					break;
				case '<':
					string += mode == 2 ? "&lt;" : "<";
					break;
				case '&':
					string += mode == 2 ? "&amp;" : "&";
					break;
				default:
					string += value[i];
				}
			}
			string += asEndValue("\"");
		}
		// HTML and 2D-raw mechanisms
		std::string asBeginTag(String c)
		{
			return mode == 2 ? asMeta(c) + "<div class='struct-block'>" : c + addLn(1);
		}
		std::string asNextTag(String c)
		{
			return mode == 2 ? asMeta(c) + "<br/>" : c + addLn(0);
		}
		std::string asEndTag(String c)
		{
			return mode == 2 ? "</div>" + asMeta(c) : addLn(-1) + c;
		}
		std::string asBeginValue(String c, bool name)
		{
			return mode == 2 ? asMeta(c) + "<span class='" + (name ? "struct-name" : "struct-value") + "'>" : c;
		}
		std::string asEndValue(String c)
		{
			return mode == 2 ? "</span>" + asMeta(c) : c;
		}
		std::string asMeta(String c, bool space = false)
		{
			return mode == 2 ? "<span class='struct-meta-char'>" + (space ? " " + c + " " : c) + "</span>" : mode == 1 && space ? " " + c + " " : c;
		}
		std::string addLn(int t)
		{
			tab += t;
			std::string s;
			if(mode == 1)
			{
				s += "\n";
				for(int i = 0; i < 2 * tab; i++)
				{
					s += " ";
				}
			}
			return s;
		}
		std::string stringHeader()
		{
			return mode == 2 ? "<style>body {background-color:lightgrey}.struct-block{margin-left:20px}.struct-meta-char{color:#330033;font-weight:bold}.struct-name{color:#000066}.struct-value{color:#006600}</style><div class'struct-block'>" : "";
		}
		std::string stringTrailer()
		{
			return mode == 2 ? "</div>" : mode == 1 ? "\n" : "";
		}
		int mode, tab;
	};
  public:
  /** Resets the data structure from a json weak-syntax string.
   * @param value The value given as a string, using weak <a href="http://json.org/">JSON</a> syntax.
   * - By default resets to an empty data structure.
   * - Value is either a string or a "format" à-la printf with the subsequent arguments.
   * - The weak-syntax allows to:
   *   - use either ':' or '=' between name and value
   *   - use either ',' or ';' as item separator
   *   - use either '"' or "'" as quote
   *   - avoid quotes for strings without space or any meta-char ':=,;]}'
   *   - accept string with '\\n' line separators (replaced by the "\\n" sequence), also manage \\b, \\r, \\t, \\f
   *   - set the value 'true' for name without explicit value
   *   - However: '\\uXXXX' unicode string sequences and the '\/' solidus escaped sequence are not managed (i.e., but simply mirrored in the string value)
   */
	void reset(String value = "",...)
	{
		clear();
		// Performs a sprintf() parsing
		std::string result;
		{
			static const int nchars = 10000;
			static char chars[nchars];
			va_list a;
			va_start(a, value);
			vsnprintf(chars, nchars, value.c_str(), a);
			va_end(a);
			result = chars;
		}
		// Now reads the String
		if(result != "")
		{
			StateReader reader;
			reader.read(*this, result);
		}
	}
  /** Resets the data structure from a json weak-syntax string stored in a file.
   * @param location The file path.
   */
	void load(String location)
	{
		std::string result;
		{
			FILE *fp = fopen(location.c_str(), "r");
			assume(fp != NULL, "IO-exception", "in State::load: %s not readable", location.c_str());
			for(char c; (c = (char)fgetc(fp)) != EOF;)
			{
				result += c;
			}
			fclose(fp);
		}
		reset(result);
	}
  private:
	// Implements the weak parsing of a json structure
	class StateReader
	{
	  public:
		void read(State & value, String string)
		{
			// Initializes the input buffer
			chars = string.c_str(), index = 0, length = string.length();
			// Clears and set the value
			value.clear();
			read_value(value);
		}
	  private:
		// String input buffer, index and length
		const char *chars;
		int index, length;
		std::string word;
		// Reads a value from the string starting at index i and set it
		void read_value(State & value)
		{
			next_space();
			switch (chars[index])
			{
			case '{':
				read_tuple_value(value);
				break;
			case '[':
				read_list_value(value);
				break;
			default:
				value = read_word(); //FUCK THIS SHIT !!!!!!
				break;
			}
		}
		// Reads a word
		String read_word()
		{
			if((chars[index] == '"') || (chars[index] == '\''))
			{
				return read_quoted_word(chars[index]);
			}
			else
			{
				return read_nospace_word();
			}
		}
		// Reads a quoted word
		String read_quoted_word(char quote)
		{
			word = "";
			for(index++; index < length && chars[index] != quote; index++)
			{
				if((chars[index] == '\\') && (index < length - 1))
				{
					index++;
					switch (chars[index])
					{
					case '"':
					case '\\':
					case '/':
						word += chars[index];
						break;
					case 'n':
						word += "\n";
						break;
					case 'b':
						word += "\b";
						break;
					case 'r':
						word += "\r";
						break;
					case 't':
						word += "\t";
						break;
					case 'f':
						word += "\f";
						break;
					default:
						word += "\\";
						word += chars[index];
					}
				}
				else
				{
					word += chars[index];
				}
			}
			if(index < length)
			{
				index++;
			}
			return word;
		}
		// Reads a no-space word
		String read_nospace_word()
		{
			int i0;
			for(i0 = index; index < length && !(isspace(chars[index]) || chars[index] == ',' || chars[index] == ';' || chars[index] == ':' || chars[index] == '=' || chars[index] == '}' || chars[index] == ']'); index++)
			{
			}
			word = std::string(chars + i0, index - i0);
			return word;
		}
		// Reads a list
		void read_list_value(State & value)
		{
			index++;
			while(true)
			{
				next_space();
				if(index >= length)
				{
					return;
				}
				if(chars[index] == ']')
				{
					index++;
					return;
				}
				State item;
				read_value(item);
				value.set(value.getLength(), item);
				next_space();
				if((index < length) && ((chars[index] == ',') || (chars[index] == ';')))
				{
					index++;
				}
			}
		}
		// Reads a tuple
		void read_tuple_value(State & value)
		{
			index++;
			while(true)
			{
				next_space();
				if(index >= length)
				{
					return;
				}
				if(chars[index] == '}')
				{
					index++;
					return;
				}
				std::string name = read_word();
				next_space();
				State item = true;
				if((index < length) && ((chars[index] == ':') || (chars[index] == '=')))
				{
					index++;
					read_value(item);
				}
				value.set(name, item);
				next_space();
				if((index < length) && ((chars[index] == ',') || (chars[index] == ';')))
				{
					index++;
				}
			}
		}
		// Shifts until the next non-space char
		void next_space()
		{
			for(; index < length && isspace(chars[index]); index++)
			{
			}
		}
	};
};
#endif
