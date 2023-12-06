#include <hyperlib/gameplay/movie.hpp>

namespace hyper
{
    bool movie::is_playing()
    {
        return this->curr_status_ >= status::starting && this->curr_status_ <= status::playing;
    }
}
