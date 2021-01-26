% 
% Copyright(c) 2021 m1bcodes
% 
% Permission is hereby granted, free of charge, to any person obtaining a copy
% of this softwareand associated documentation files(the "Software"), to deal
% in the Software without restriction, including without limitation the rights
% to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
% copies of the Software, and to permit persons to whom the Software is
% furnished to do so, subject to the following conditions :
% 
% The above copyright noticeand this permission notice shall be included in all
% copies or substantial portions of the Software.
% 
% THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
% IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
% FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
% AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
% LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
% OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
% SOFTWARE.
% 
% fork me on GitHub: https://github.com/m1bcodes/IIR_Ringbuffer_Filter
%
% Example:
% b=[0.132937288987445   0.531749155949779   0.797623733924668   0.531749155949779   0.132937288987445] * 1e-4;
% a=[1.000000000000000  -3.671729089161935   5.067998386734188  -3.115966925201744   0.719910327291871];
% t=linspace(0, 1,1000);
% step = [0.2*ones(1, length(t)/2) ones(1, length(t)/2)] ;
% 
% y1 = filter(b,a,step);
% y2 = zeros(size(t));
% f = FilterRingBuffer(b,a);
% for i1=1:length(t)
%     y2(i1) = f.push(step(i1));
% end
% 
% figure;
% plot(t, y1, '.-', t, y2, 'x-');
% legend('filter()', 'RingBuffer');

classdef FilterRingBuffer < handle
    methods
        function obj = FilterRingBuffer(coef_b, coef_a)
            obj.a = coef_a;
            obj.b = coef_b;
            obj.len = max(length(coef_a), length(coef_b));
            obj.x = zeros(1,obj.len);
            obj.y = zeros(1,obj.len);
            obj.idx = 0;
        end
        
        function obj = clear(obj)
            obj.x = zeros(1,obj.len);
            obj.y = zeros(1,obj.len);
            obj.idx = 0;
        end
        
        function outVal=push(obj, inVal)
            obj.x(1+obj.idx)=inVal;
            s = obj.b(1) * obj.x(1+obj.idx);
            for i2 = 2:length(obj.b)
                s = s + obj.b(i2) * obj.x(1+mod(obj.idx-i2+1+obj.len,obj.len));
            end
            for i2=2:length(obj.a)
                s = s - obj.a(i2) * obj.y(1+mod(obj.idx-i2+1+obj.len,obj.len));
            end
            obj.y(1+obj.idx)=s;
            obj.idx=mod(obj.idx+1,obj.len);
            outVal=obj.y(1+obj.idx);
        end
    end
    
    properties (Access = private)
        a
        b
        len
        x
        y
        idx
    end
end

