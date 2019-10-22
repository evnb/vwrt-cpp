# Video Waste Reduction Tool

By: Evan Binder, Nicholas Dodd

## Usage
### ./vidsum input.mp4 output.mp4 talking_speed quiet_speed
  It is optional to omit both `talking_speed` and `quiet_speed`, but not just one. (default values are 1.0 and 2.0 respectively)
  
  _Note:_
  - _vwrt does not accept flags at this time_
  - _vwrt should work with any video file type_
  - _vwrt was only tested on MacOS, but we imagine it would work on other OS's_

## Troubleshooting
- VWRT seems to run aggressively slower on longer input videos. To speed this up for long videos, try [splitting them](https://unix.stackexchange.com/questions/1670/how-can-i-use-ffmpeg-to-split-mpeg-video-into-10-minute-chunks) into shorter segments, running VWRT on each segment, and then [stitching the segments](https://stackoverflow.com/questions/7333232/how-to-concatenate-two-mp4-files-using-ffmpeg) back together. This will eventually be implemented into VWRT untill we have a better fix. 
- Make sure you have ffmpeg [installed](https://superuser.com/questions/624561/install-ffmpeg-on-os-x), upgraded, and [in PATH](https://superuser.com/questions/324616/how-should-i-set-the-path-variable-on-my-mac-so-the-hombrew-installed-tools-are)
- Avoid spaces in names and directories
- If you get a complicated error message, it is likely a problem with ffmpeg and not vwrt. Make sure ffmpeg is up to date and google the error if it persists.

## Examples
**When tested on two lecture review videos, VWRT successfully reduced watchtime by about 31%.**
- [Short video comparison](https://vimeo.com/368077476)
## Explanation
The objective of the Video Waste Reduction Tool (VWRT) is to optimize long informational videos to waste as little of the viewer’s time as possible. Specifically, VWRT is aimed towards academic review videos. Review videos have become commonplace in the recent years, as more academic institutions follow the likes of MIT’s OpenCourseWare (OCW), in recording lecture videos to be posted online for public consumption. The movement of OCW has made educational resources more abundant and accessible than ever before, but these videos are far from perfect. Quality of academic videos is too often an afterthought, and a number of technological and content-related factors can obstruct the learning experience for viewers. Technological problems, such as too-low image resolution, or unintelligible audio, are experienced across many types of video and thus can be remedied with various existing software packages. However, problems relating to content of lecture videos are more niche and cannot be remedied by current commonplace software. These videos are often not rehearsed and as a result, there may be long sections of pauses or writing on the board. The way VWRT seeks to solve this problem is to find sections of a video where audio is below a certain threshold, and then fast-forward these sections to optimize the flow of information from the video to the viewer. 

The achieved results of the program are promising. In conducting tests with shorter videos, VWRT was able to identify parts of a video with low audio and fast-forward through them. In comparing the original video and processed video, it was found that the processed video was considerably shorter depending on the amount of non-audible sections. The processed video, however, seems to have slightly diminished image quality. In sum, the program accomplishes the set task by decreasing the overall viewing time of extraneous footage.

## Related Resources
- [Jumpcutter]:(https://github.com/carykh/jumpcutter) `Jumpcutter` is a watch-time reduction tool also built around `ffmpeg`, but in order to achieve theoretically smoother edits it saves each frame as a png, taking up a lot of storage space. _(Cary was also a big inspiration for me to get back to coding when I was in a rut, and I promise we didn't copy your idea. -Evan, a big fan)_
